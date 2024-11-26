#include <SFML/Network.hpp>
#include <iostream>
#include <thread>
#include <vector>
#include <unordered_map>
#include <functional>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>

// 명령어 처리 핸들러
std::unordered_map<std::string, std::function<void(const std::string&, sf::TcpSocket*)>> commandHandlers;

// 작업 큐와 스레드 풀 관련 변수
std::queue<sf::TcpSocket*> taskQueue;
std::mutex queueMutex;
std::condition_variable condition;
std::atomic<bool> stop(false); // 서버 종료 플래그

// 매칭 대기열 관련 변수
std::queue<sf::TcpSocket*> matchingQueue; // 매칭 대기열
std::mutex matchingMutex; // 매칭 대기열 보호용 뮤텍스

// 함수 선언
void ThreadWorker();
void HandleClient(sf::TcpSocket* client);
void MonitorConsoleInput();
void StartGame(const std::string& data, sf::TcpSocket* client);
void SetTurn(const std::string& data, sf::TcpSocket* client);
void HandleMatchingRequest(const std::string& data, sf::TcpSocket* client);
void StartMatching(sf::TcpSocket* client);

// 스레드 풀에서 실행되는 작업
void ThreadWorker() {
    while (true) {
        sf::TcpSocket* client = nullptr;

        {
            std::unique_lock<std::mutex> lock(queueMutex);
            condition.wait(lock, [] { return !taskQueue.empty() || stop.load(); });

            if (stop && taskQueue.empty())
                break;

            client = taskQueue.front();
            taskQueue.pop();
        }

        if (client) {
            HandleClient(client);
        }
    }
}

// 클라이언트 처리 함수
void HandleClient(sf::TcpSocket* client) {
    sf::Packet packet;
    std::string message = "Welcome to the room!";
    packet << message;

    if (client->send(packet) != sf::Socket::Done) {
        std::cerr << "Error sending message to client" << std::endl;
        return;
    }

    while (true) {
        sf::Packet receivedPacket;
        if (client->receive(receivedPacket) == sf::Socket::Done) {
            std::string clientMessage;
            receivedPacket >> clientMessage;

            std::cout << "Client says: " << clientMessage << std::endl;

            auto it = commandHandlers.find(clientMessage);
            if (it != commandHandlers.end()) {
                it->second(clientMessage, client);
            }
            else {
                std::cerr << "Unknown command: " << clientMessage << std::endl;
            }
        }
        else {
            std::cerr << "Client disconnected." << std::endl;
            break;
        }
    }

    client->disconnect();
    delete client;
}

// 매칭 시작 요청 처리
void HandleMatchingRequest(const std::string& data, sf::TcpSocket* client) {
    if (data == "START_MATCHING") {
        StartMatching(client);
    }
}

// 매칭 로직
void StartMatching(sf::TcpSocket* client) {
    std::lock_guard<std::mutex> lock(matchingMutex);

    matchingQueue.push(client);

    if (matchingQueue.size() >= 2) {
        sf::TcpSocket* player1 = matchingQueue.front();
        matchingQueue.pop();
        sf::TcpSocket* player2 = matchingQueue.front();
        matchingQueue.pop();

        std::cout << "매칭 완료: 두 플레이어 연결됨." << std::endl;

        sf::Packet packet1;
        packet1 << "MATCH_FOUND" << "You are matched with Player 2";
        player1->send(packet1);

        sf::Packet packet2;
        packet2 << "MATCH_FOUND" << "You are matched with Player 1";
        player2->send(packet2);
    }
    else {
        std::cout << "매칭 대기 중..." << std::endl;
    }
}

// 게임 시작 명령 처리
void StartGame(const std::string& data, sf::TcpSocket* client) {
    std::cout << "Game started for client: " << client << " with data: " << data << std::endl;
}

// 턴 설정 명령 처리
void SetTurn(const std::string& data, sf::TcpSocket* client) {
    std::cout << "Turn set for client: " << client << " with data: " << data << std::endl;
}

// 콘솔 입력 모니터링
void MonitorConsoleInput() {
    std::string input;
    while (true) {
        std::getline(std::cin, input);
        if (input == "exit") {
            std::cout << "서버 종료 요청을 받았습니다." << std::endl;

            {
                std::lock_guard<std::mutex> lock(queueMutex);
                stop = true;
            }
            condition.notify_all();
            break;
        }
    }
}

int main() {
    commandHandlers["START_GAME"] = StartGame;
    commandHandlers["SET_TURN"] = SetTurn;
    commandHandlers["START_MATCHING"] = HandleMatchingRequest;

    sf::TcpListener listener;
    if (listener.listen(53000) != sf::Socket::Done) {
        std::cerr << "Error: Could not listen on port 53000" << std::endl;
        return -1;
    }

    std::cout << "서버가 실행 중입니다. 클라이언트를 기다립니다..." << std::endl;

    const int THREAD_COUNT = 4;
    std::vector<std::thread> threadPool;
    for (int i = 0; i < THREAD_COUNT; ++i) {
        threadPool.emplace_back(ThreadWorker);
    }

    std::thread consoleThread(MonitorConsoleInput);

    while (!stop) {
        sf::TcpSocket* client = new sf::TcpSocket();

        if (listener.accept(*client) == sf::Socket::Done) {
            std::cout << "새 클라이언트가 연결되었습니다!" << std::endl;
            {
                std::lock_guard<std::mutex> lock(queueMutex);
                taskQueue.push(client);
            }
            condition.notify_one();
        }
        else {
            delete client;
        }
    }

    {
        std::lock_guard<std::mutex> lock(queueMutex);
        stop = true;
    }
    condition.notify_all();

    for (auto& thread : threadPool) {
        if (thread.joinable()) {
            thread.join();
        }
    }

    if (consoleThread.joinable()) {
        consoleThread.join();
    }

    std::cout << "서버가 종료되었습니다." << std::endl;
    return 0;
}

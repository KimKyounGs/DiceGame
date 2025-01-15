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
#include <fstream>

// 명령어 처리 핸들러
std::unordered_map<std::string, std::function<void(const std::string&, sf::TcpSocket*)>> commandHandlers;

// 작업 큐 및 스레드 풀 관련 변수
std::queue<sf::TcpSocket*> taskQueue;
std::mutex queueMutex;
std::condition_variable condition;
std::atomic<bool> stop(false); // 서버 종료 플래그

// 매칭 대기열 관련 변수
std::queue<sf::TcpSocket*> matchingQueue;
std::mutex matchingMutex;

// 로깅 시스템
std::mutex logMutex;
std::ofstream logFile("server.log", std::ios::app);

// 함수 선언
void ThreadWorker();
void HandleClient(sf::TcpSocket* client);
void MonitorConsoleInput();
void StartGame(const std::string& data, sf::TcpSocket* client);
void SetTurn(const std::string& data, sf::TcpSocket* client);
void HandleMatchingRequest(const std::string& data, sf::TcpSocket* client);
void StartMatching(sf::TcpSocket* client);

// 로그 기록 함수
void Log(const std::string& message) {
    std::lock_guard<std::mutex> lock(logMutex);
    logFile << message << std::endl;
    std::cout << message << std::endl;
}

// 스레드 풀에서 실행되는 작업
void ThreadWorker() {
    while (true) {
        sf::TcpSocket* client = nullptr;

        {
            std::unique_lock<std::mutex> lock(queueMutex);
            condition.wait(lock, [] { return !taskQueue.empty() || stop.load(); });

            if (stop && taskQueue.empty()) break;

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
    packet << "Welcome to the room!";

    if (client->send(packet) != sf::Socket::Done) {
        Log("Error sending message to client.");
        return;
    }

    while (true) {
        sf::Packet receivedPacket;
        if (client->receive(receivedPacket) == sf::Socket::Done) {
            std::string clientMessage;
            receivedPacket >> clientMessage;

            Log("Client says: " + clientMessage);

            auto it = commandHandlers.find(clientMessage);
            if (it != commandHandlers.end()) {
                it->second(clientMessage, client);
            }
            else {
                Log("Unknown command: " + clientMessage);
            }
        }
        else {
            Log("Client disconnected.");
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

        Log("매칭 완료: 두 플레이어 연결됨.");

        sf::Packet packet1;
        packet1 << "MATCH_FOUND" << "You are matched with Player 2";
        player1->send(packet1);

        sf::Packet packet2;
        packet2 << "MATCH_FOUND" << "You are matched with Player 1";
        player2->send(packet2);
    }
    else {
        Log("매칭 대기 중...");
    }
}

// 게임 시작 명령 처리
void StartGame(const std::string& data, sf::TcpSocket* client) {
    Log("Game started for client: " + std::to_string(reinterpret_cast<std::uintptr_t>(client)) + " with data: " + data);
}

// 턴 설정 명령 처리
void SetTurn(const std::string& data, sf::TcpSocket* client) {
    Log("Turn set for client: " + std::to_string(reinterpret_cast<std::uintptr_t>(client)) + " with data: " + data);
}

// 콘솔 입력 모니터링
void MonitorConsoleInput() {
    std::string input;
    while (true) {
        std::getline(std::cin, input);
        if (input == "exit") {
            Log("서버 종료 요청을 받았습니다.");

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
    // 명령어 핸들러 등록
    commandHandlers["START_GAME"] = StartGame;
    commandHandlers["SET_TURN"] = SetTurn;
    commandHandlers["START_MATCHING"] = HandleMatchingRequest;

    sf::TcpListener listener;
    if (listener.listen(53000) != sf::Socket::Done) {
        Log("Error: Could not listen on port 53000");
        return -1;
    }

    Log("서버가 실행 중입니다. 클라이언트를 기다립니다...");

    // 스레드 풀 생성
    const int THREAD_COUNT = 4;
    std::vector<std::thread> threadPool;
    for (int i = 0; i < THREAD_COUNT; ++i) {
        threadPool.emplace_back(ThreadWorker);
    }

    std::thread consoleThread(MonitorConsoleInput);

    // 클라이언트 대기
    while (!stop) {
        sf::TcpSocket* client = new sf::TcpSocket();

        if (listener.accept(*client) == sf::Socket::Done) {
            Log("새 클라이언트가 연결되었습니다!");
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

    // 서버 종료 처리
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

    Log("서버가 종료되었습니다.");
    logFile.close();
    return 0;
}

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
std::unordered_map<std::string, std::function<void(const std::string&)>> commandHandlers;

// 작업 큐와 스레드 풀 관련 변수
std::queue<sf::TcpSocket*> taskQueue;
std::mutex queueMutex;
std::condition_variable condition;
std::atomic<bool> stop(false); // 서버 종료 플래그 (atomic으로 변경)

// 함수 선언 추가
void ThreadWorker();
void HandleClient(sf::TcpSocket* client); // 여기 추가
void MonitorConsoleInput();
void StartGame(const std::string& data);
void SetTurn(const std::string& data);

// 스레드 풀에서 실행되는 작업
void ThreadWorker() {
    while (true) {
        sf::TcpSocket* client = nullptr;

        {
            std::unique_lock<std::mutex> lock(queueMutex);
            // 작업 큐에 작업이 들어올 때까지 대기
            condition.wait(lock, [] { return !taskQueue.empty() || stop.load(); });

            if (stop && taskQueue.empty())
                break; // 서버 종료 시 루프 탈출

            // 작업 가져오기
            client = taskQueue.front();
            taskQueue.pop();
        }

        if (client) {
            // 클라이언트 처리
            HandleClient(client);
        }
    }
}

// 클라이언트와 통신하는 함수
void HandleClient(sf::TcpSocket* client) {
    sf::Packet packet;
    std::string message = "Welcome to the room!";
    packet << message;

    // 클라이언트에게 환영 메시지 전송
    if (client->send(packet) != sf::Socket::Done) {
        std::cerr << "Error sending message to client" << std::endl;
    }
    else {
        std::cout << "클라이언트에게 메시지 전송 완료!" << std::endl;
    }

    // 클라이언트로부터 메시지 수신
    while (true) {
        sf::Packet receivedPacket;
        if (client->receive(receivedPacket) == sf::Socket::Done) {
            std::string clientMessage;
            receivedPacket >> clientMessage;

            std::cout << "Client says: " << clientMessage << std::endl;

            // 명령어 처리
            auto it = commandHandlers.find(clientMessage);
            if (it != commandHandlers.end()) {
                it->second(clientMessage); // 해당 명령어 처리
            }
            else {
                std::cerr << "Unknown command: " << clientMessage << std::endl;
            }
        }
        else {
            std::cerr << "클라이언트와의 연결이 끊어졌습니다." << std::endl;
            break;
        }
    }

    client->disconnect(); // 클라이언트 연결 해제
    delete client;        // 메모리 해제
}

// 명령 처리 함수들
void StartGame(const std::string& data) {
    std::cout << "Game started with: " << data << std::endl;
}

void SetTurn(const std::string& data) {
    std::cout << "Turn set to: " << data << std::endl;
}

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
            condition.notify_all(); // 모든 대기 중인 스레드 깨움
            break;
        }
    }
}

int main() {
    // 명령어와 처리 함수 맵핑
    commandHandlers["START_GAME"] = StartGame;
    commandHandlers["SET_TURN"] = SetTurn;

    sf::TcpListener listener;
    if (listener.listen(53000) != sf::Socket::Done) {
        std::cerr << "Error: Could not listen on port 53000" << std::endl;
        return -1;
    }

    std::cout << "서버가 실행 중입니다. 클라이언트를 기다립니다..." << std::endl;

    // 스레드 풀 생성
    const int THREAD_COUNT = 4; // 스레드 풀 크기
    std::vector<std::thread> threadPool;
    for (int i = 0; i < THREAD_COUNT; ++i) {
        threadPool.emplace_back(ThreadWorker);
    }

    // 콘솔 입력을 처리할 스레드 시작
    std::thread consoleThread(MonitorConsoleInput);

    // 클라이언트 연결 수락 루프
    while (!stop) {
        sf::TcpSocket* client = new sf::TcpSocket();

        // 새로운 클라이언트 수락
        if (listener.accept(*client) == sf::Socket::Done) {
            std::cout << "새 클라이언트가 연결되었습니다!" << std::endl;
            {
                std::lock_guard<std::mutex> lock(queueMutex);
                taskQueue.push(client); // 작업 큐에 추가
            }

            condition.notify_one(); // 대기 중인 스레드 하나를 깨움
        }
        else {
            delete client; // 실패 시 메모리 해제
        }
    }

    // 서버 종료 신호
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        stop = true;
    }
    condition.notify_all(); // 모든 대기 중인 스레드 깨움

    // 스레드 풀 종료
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

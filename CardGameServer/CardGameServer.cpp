#include <SFML/Network.hpp>
#include <iostream>
#include <thread>
#include <vector>

// 클라이언트와 통신하는 함수
void HandleClient(sf::TcpSocket* client) {
    sf::Packet packet;
    std::string message = "Welcome to the room!";
    // 패킷에 메시지 추가
    packet << message;

    // 메시지 전송
    if (client->send(packet) != sf::Socket::Done) {
        std::cerr << "Error sendicmm,ㅡㅡng message to client" << std::endl;
    }
    else
    {
        std::cout << "클라이언트한테 메세지 보냄!" << std::endl;
    }

    // 클라이언트로부터 메시지 수신
    while (true) {
        sf::Packet receivedPacket;
        if (client->receive(receivedPacket) == sf::Socket::Done) {
            std::string clientMessage;
            receivedPacket >> clientMessage;
            std::cout << "Client says: " << clientMessage << std::endl;
        }
        else {
            std::cerr << "클라이언트랑 서버가 끊겼음.. ㅠㅠ" << std::endl;
            break;
        }
    }

    client->disconnect();  // 연결 해제
}

int main() {
    sf::TcpListener listener;
    if (listener.listen(53000) != sf::Socket::Done) {
        std::cerr << "Error: Could not listen on port 53000" << std::endl;
        return -1;
    }

    std::cout << "Server is running. Waiting for connections..." << std::endl;

    // 클라이언트 연결을 처리하는 스레드들을 저장할 벡터
    std::vector<std::thread> clientThreads;

    while (true) {
        sf::TcpSocket* client = new sf::TcpSocket;

        // 새로운 클라이언트 수락
        // 스레드를 생성해서 클라이언트를 독립적으로 실행함.
        if (listener.accept(*client) == sf::Socket::Done) {
            std::cout << "New client connected!" << std::endl;
            clientThreads.emplace_back(std::thread(HandleClient, client));  // 새로운 스레드에서 클라이언트 처리
        }
        else {
            delete client;
        }
    }

    // 스레드가 종료될 때까지 대기 -> 메인 스레드는 나머지 스레드가 종료될 때까지 기다리기.
    for (auto& thread : clientThreads) {
        if (thread.joinable()) {
            std::cout << "Join중입니다." << std::endl;
            thread.join();
        }
    }

    return 0;
}
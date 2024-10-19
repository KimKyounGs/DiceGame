#include <SFML/Network.hpp>
#include <iostream>

int main() {
    sf::TcpSocket socket;

    // 서버에 연결
    if (socket.connect("127.0.0.1", 53000) != sf::Socket::Done) {
        std::cerr << "Error: Could not connect to server" << std::endl;
        return -1;
    }

    std::cout << "Connected to server!" << std::endl;

    // 서버로부터 메시지 수신
    sf::Packet packet;
    std::string message;
    // sf::Socket::Done은 데이터가 정상적으로 수신되었음을 의미합니다.
    if (socket.receive(packet) == sf::Socket::Done) {
        packet >> message; // 패킷에서 메시지 추출
        std::cout << "Message from server: " << message << std::endl;
    }
    else {
        std::cerr << "Error receiving message from server" << std::endl;
    }

    while (true)
    {
        // 서버로 메시지 전송
        sf::Packet sendPacket;
        std::string messageToSend = "안녕 서버야~";
        std::cin >> messageToSend;
        sendPacket << messageToSend;  // 메시지를 패킷에 추가

        if (socket.send(sendPacket) != sf::Socket::Done) {
            std::cerr << "Error: Could not send message to server" << std::endl;
        }
        else {
            std::cout << "서버한테 메세지 보냄 : " << messageToSend << std::endl;
        }

        if (messageToSend == "bye")
        {
            std::cout<< "클라이언트 종료" << std::endl;
            break;
        }
    }


    return 0;
}
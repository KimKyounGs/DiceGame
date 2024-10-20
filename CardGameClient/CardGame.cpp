#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>

bool SocketConnet()
{
    sf::TcpSocket socket;

    // 서버에 연결
    if (socket.connect("127.0.0.1", 53000) != sf::Socket::Done) {
        std::cerr << "Error: Could not connect to server" << std::endl;
        return false;
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

    // 무한으로 메시지 전달.
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
            std::cout << "클라이언트 종료" << std::endl;
            break;
        }
    }

    return false;
}

bool isButtonClicked(const sf::RectangleShape& button, sf::Vector2i mousePos) {
    return button.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos));
}

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Main Menu");

    // 시작 버튼 만들기
    sf::RectangleShape startButton(sf::Vector2f(200, 100));
    startButton.setPosition(300, 250);
    startButton.setFillColor(sf::Color::Green);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    if (isButtonClicked(startButton, mousePos)) {
                        std::cout << "Start Button Clicked!" << std::endl;
                        // 서버에 연결하고 매칭 시작
                        // 이후 게임 화면으로 전환
                    }
                }
            }
        }

        window.clear();
        window.draw(startButton);  // 시작 버튼 그리기
        window.display();
    }

    return 0;
}
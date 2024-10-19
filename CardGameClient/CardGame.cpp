#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>

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
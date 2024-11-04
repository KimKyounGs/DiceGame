#include "MainMenu.h"

MainMenu::MainMenu() {
    startButton.setSize(sf::Vector2f(200, 100));
    startButton.setPosition(300, 250);
    startButton.setFillColor(sf::Color::Green);
}

bool MainMenu::HandleEvent(const sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2i mousePos = sf::Mouse::getPosition();
            return IsButtonClicked(mousePos);
        }
    }
    return false;
}

void MainMenu::Draw(sf::RenderWindow& window) {
    window.draw(startButton);
}

bool MainMenu::IsButtonClicked(sf::Vector2i mousePos) const {
    return startButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos));
}

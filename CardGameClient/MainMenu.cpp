#include "MainMenu.h"
#include <iostream>

MainMenu::MainMenu() {
    m_startButton.setSize(sf::Vector2f(200, 200));
    m_startButton.setPosition(300, 250);
    
    if (!m_startButtonnTexture.loadFromFile("Image\\StartButton.png"))
    {
        std::cerr << "m_startButtonnTexture 로드 실패!" << std::endl;
    }
    else
    {
        m_startButton.setTexture(&m_startButtonnTexture);
    }
}

bool MainMenu::HandleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            return IsButtonClicked(mousePos);
        }
    }
    return false;
}

void MainMenu::Draw(sf::RenderWindow& window) {
    window.draw(m_startButton);
}

bool MainMenu::IsButtonClicked(sf::Vector2i mousePos) const {
    return m_startButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos));
}

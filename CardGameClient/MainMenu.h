#pragma once
#ifndef MAINMENU_H
#define MAINMENU_H

#include <SFML/Graphics.hpp>

class MainMenu {
public:
    MainMenu();
    bool HandleEvent(const sf::Event& event, const sf::RenderWindow&);
    void Draw(sf::RenderWindow& window);

private:
    sf::RectangleShape m_startButton;
    sf::Texture m_startButtonnTexture;

private:
    bool IsButtonClicked(sf::Vector2i mousePos) const;

};

#endif // MAINMENU_H

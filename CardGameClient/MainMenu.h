#pragma once
#ifndef MAINMENU_H
#define MAINMENU_H

#include <SFML/Graphics.hpp>

class MainMenu {
public:
    MainMenu();
    bool HandleEvent(const sf::Event& event);
    void Draw(sf::RenderWindow& window);

private:
    sf::RectangleShape startButton;
    bool IsButtonClicked(sf::Vector2i mousePos) const;
};

#endif // MAINMENU_H

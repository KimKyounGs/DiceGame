#pragma once
#ifndef MAINGAME_H
#define MAINGAME_H

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

namespace Game
{
    enum class State { Ready, Game };
}

class MainGame {

private:

public:
    MainGame();
    void Draw(sf::RenderWindow& window);
    void Update(sf::TcpSocket& socket);
};

#endif // MAINGAME_H
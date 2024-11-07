#pragma once
#ifndef MAINGAME_H
#define MAINGAME_H

#include <SFML/Graphics.hpp>

namespace Game
{
    enum class State { Ready, Game };
}

class MainGame {

private:

public:
    void Draw(sf::RenderWindow& window);
};

#endif // MAINGAME_H
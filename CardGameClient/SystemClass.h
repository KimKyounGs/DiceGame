#pragma once
#ifndef _SYSTEMCLASS_H
#define _SYSTEMCLASS_H

/// <summary>
/// include
/// </summary>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "MainMenu.h"
#include "Game.h"
#include "Chat.h"

/// <summary>
/// Class Name = SystemClass
/// </summary>
class SystemClass
{
public:
    SystemClass();
    SystemClass(const SystemClass&);
    ~SystemClass();

    void Run();

private:
    enum class State { MainMenu, GameScreen };
    State currentState;
    bool isConnected;

    sf::RenderWindow window;
    MainMenu mainMenu;
    GameScreen gameScreen;
    Chat chat;
    sf::TcpSocket socket;

    void ProcessEvents();
    void Update();
    void Render();
    bool ConnectToServer();
};


#endif // !_SYSTEMCLASS_H

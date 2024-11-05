#pragma once
#ifndef _SYSTEMCLASS_H
#define _SYSTEMCLASS_H

/// <summary>
/// include
/// </summary>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "MainMenu.h"
#include "MainGame.h"
#include "Chat.h"

/// <summary>
/// Class Name = SystemClass
/// </summary>
class SystemClass
{
public:
    SystemClass();

    void Run();

private:
    enum class State { MainMenu, MainGame};
    State m_currentState;
    bool m_isConnected;

    sf::RenderWindow m_window;
    MainMenu m_mainMenu;
    MainGame m_mainGame;
    Chat m_chat;
    sf::TcpSocket m_socket;

private:
    void ProcessEvents();
    void Update();
    void Render();
    bool ConnectToServer();
};


#endif // !_SYSTEMCLASS_H

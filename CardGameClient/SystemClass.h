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

namespace System
{
    enum class State { MainMenu, MainGame};
}

/// <summary>
/// Class Name = SystemClass
/// </summary>
class SystemClass
{
private:
    System::State m_currentState;
    bool m_isConnected;

    sf::RenderWindow m_window;
    MainMenu m_mainMenu;
    MainGame m_mainGame;
    Chat m_chat;
    sf::TcpSocket m_socket;

public:
    SystemClass();

    void Run();

private:
    void ProcessEvents();
    void Update();
    void Render();
    bool ConnectToServer();
};


#endif // !_SYSTEMCLASS_H

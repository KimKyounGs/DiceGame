#include "SystemClass.h"
#include "iostream"

SystemClass::SystemClass() : m_window(sf::VideoMode(800, 600), "CardGame"),
							 m_currentState(State::MainMenu),
							 m_isConnected(false) {}

void SystemClass::Run()
{
	while (m_window.isOpen())
	{
		ProcessEvents();
		Update();
		Render();
	}
}

void SystemClass::ProcessEvents()
{
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed) m_window.close();

		if (m_currentState == State::MainMenu)
		{
			if (m_mainMenu.HandleEvent(event, m_window))
			{
				if (ConnectToServer())
				{
					std::cout << "서버 연결" << std::endl;
					m_currentState = State::MainGame;
				}
			}
		}
		else if (m_currentState == State::MainGame)
		{
			//gameScreen.HandleEvent();
			//m_chat.HandleEvent(event, m_socket);
		}
	}
}

void SystemClass::Update()
{
	if (m_currentState == State::MainMenu)
	{
		//mainMenu.Update();
	}
	else if (m_currentState == State::MainGame) {
		// m_chat.Update(m_socket);
	}
}

void SystemClass::Render()
{
	m_window.clear();
	if (m_currentState == State::MainMenu) {
		m_mainMenu.Draw(m_window);
	}
	else if (m_currentState == State::MainGame) {
		m_mainGame.Draw(m_window);
		m_chat.Draw(m_window);
	}
	m_window.display();
}

bool SystemClass::ConnectToServer()
{
	if (m_socket.connect("127.0.0.1", 53000) != sf::Socket::Done) {
		std::cerr << "Error: Could not connect to server" << std::endl;
		return false;
	}
	m_isConnected = true;
	std::cout << "Connected to server!" << std::endl;
	m_chat.StartReceiving(m_socket);  // m_chat의 수신 스레드 시작
	return true;
}

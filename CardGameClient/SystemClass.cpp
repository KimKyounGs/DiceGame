#include "SystemClass.h"
#include "iostream"

SystemClass::SystemClass() : m_window(sf::VideoMode(800, 600), "CardGame"),
							 m_currentState(System::State::MainMenu),
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

		if (m_currentState == System::State::MainMenu)
		{
			if (m_mainMenu.HandleEvent(event, m_window))
			{
				if (ConnectToServer())
				{
					std::cout << "서버 연결" << std::endl;
					m_currentState = System::State::MainGame;
				}
			}
		}
		else if (m_currentState == System::State::MainGame)
		{
			//gameScreen.HandleEvent();
			m_chat.HandleEvent(event, m_gameSocket);
		}
	}
}

void SystemClass::Update()
{
	if (m_currentState == System::State::MainMenu)
	{
		// mainMenu.Update();
	}
	else if (m_currentState == System::State::MainGame) {
		m_mainGame.Update(m_gameSocket);
		m_chat.Update(m_chatSocket);
	}
}

void SystemClass::Render()
{
	m_window.clear();
	if (m_currentState == System::State::MainMenu) {
		m_mainMenu.Draw(m_window);	// 메인메뉴 그리기
	}
	else if (m_currentState == System::State::MainGame) {
		m_mainGame.Draw(m_window);	// 메인게임 그리기
		m_chat.Draw(m_window);	// 채팅 그리기
	}
	m_window.display();
}

bool SystemClass::ConnectToServer()
{
	if (m_menuSocket.connect("127.0.0.1", 53000) != sf::Socket::Done) {
		std::cerr << "Error: Could not connect to server" << std::endl;
		m_isConnected = false;
		return false;
	}
	m_isConnected = true;
	std::cout << "Connected to server!" << std::endl;
	m_chat.StartReceiving(m_menuSocket);	// m_chat의 수신 스레드 시작
	return true;
}

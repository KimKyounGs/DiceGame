#include "SystemClass.h"
#include "iostream"

SystemClass::SystemClass() : window(sf::VideoMode(1600, 900), "CardGame"),
							 currentState(State::MainMenu),
							 isConnected(false) {}

void SystemClass::Run()
{
	while (window.isOpen())
	{
		ProcessEvents();
		Update();
		Render();
	}
}

void SystemClass::ProcessEvents()
{
	sf::Event event;
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed) window.close();

		if (currentState == State::MainMenu)
		{
			if (mainMenu.HandleEvent(event))
			{
				if (ConnectToServer())
				{
					currentState = State::GameScreen;
				}
			}
		}

		else if (currentState == State::GameScreen)
		{
			chat.HandleEvent(event, socket);
		}
	}
}

void SystemClass::Update()
{
	if (currentState == State::GameScreen) {
		chat.Update(socket);
	}
}

void SystemClass::Render()
{
	window.clear();
	if (currentState == State::MainMenu) {
		mainMenu.Draw(window);
	}
	else if (currentState == State::GameScreen) {
		gameScreen.draw(window);
		chat.Draw(window);
	}
	window.display();
}

bool SystemClass::ConnectToServer()
{
	if (socket.connect("127.0.0.1", 53000) != sf::Socket::Done) {
		std::cerr << "Error: Could not connect to server" << std::endl;
		return false;
	}
	isConnected = true;
	std::cout << "Connected to server!" << std::endl;
	return true;
}

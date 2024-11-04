#pragma once
#ifndef CHAT_H
#define CHAT_H

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <vector>
#include <string>

class Chat {
public:
    Chat();
    void HandleEvent(const sf::Event& event, sf::TcpSocket& socket);
    void Update(sf::TcpSocket& socket);
    void Draw(sf::RenderWindow& window);

private:
    sf::RectangleShape inputBox;
    sf::Text inputText;
    sf::Font font;
    std::string currentInput;
    std::vector<std::string> messages;

    void SendMessage(sf::TcpSocket& socket);
};

#endif // CHAT_H
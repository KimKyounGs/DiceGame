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
    sf::RectangleShape m_inputBox;
    sf::Text m_inputText;
    sf::Font m_font;
    std::string m_currentInput;
    std::vector<std::string> m_messages;

    void SendMessage(sf::TcpSocket& socket);
};

#endif // CHAT_H
#pragma once
#ifndef CHAT_H
#define CHAT_H

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <vector>
#include <string>
#include <thread>
#include <mutex>

class Chat {
public:
    Chat();
    ~Chat();
    void HandleEvent(const sf::Event& event, sf::TcpSocket& socket);
    void Update(sf::TcpSocket& socket);
    void Draw(sf::RenderWindow& window);
    void StartReceiving(sf::TcpSocket& socket);

private:
    sf::RectangleShape m_inputBox;
    sf::Text m_inputText;
    sf::Font m_font;
    std::string m_currentInput;
    std::vector<std::string> m_messages;
    std::vector<std::string> m_receivedMessages;
    std::thread m_receivingThread;
    std::mutex m_mutex; // m_messages와 m_receivedMessages 보호용 뮤텍스
    bool m_isChatting;

    void SendMessage(sf::TcpSocket& socket);
    void ReceiveMessage(sf::TcpSocket& socket);
};

#endif // CHAT_H
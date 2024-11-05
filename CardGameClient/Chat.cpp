#include "Chat.h"
#include <iostream>

Chat::Chat() {
    m_inputBox.setSize(sf::Vector2f(400, 30));
    m_inputBox.setPosition(50, 550);
    m_inputBox.setFillColor(sf::Color::Cyan);

    if (!m_font.loadFromFile("Font\\RightFont.ttf")) {
        std::cerr << "Font load error" << std::endl;
    }

    m_inputText.setFont(m_font);
    m_inputText.setCharacterSize(20);
    m_inputText.setFillColor(sf::Color::Black);
    m_inputText.setPosition(55, 555);
}

void Chat::HandleEvent(const sf::Event& event, sf::TcpSocket& socket) {
    std::cout << "ChatHandleEvent start" << std::endl;
    if (event.type == sf::Event::TextEntered) {
        if (event.text.unicode == '\r') {
            SendMessage(socket);
        }
        else if (event.text.unicode == '\b') {
            if (!m_currentInput.empty()) {
                m_currentInput.pop_back();
            }
        }
        else {
            m_currentInput += static_cast<char>(event.text.unicode);
        }
        m_inputText.setString(m_currentInput);
    }
    std::cout << "ChatHandleEvent end" << std::endl;
}

void Chat::Update(sf::TcpSocket& socket) {
    sf::Packet packet;
    if (socket.receive(packet) == sf::Socket::Done) {
        std::string message;
        packet >> message;
        m_messages.push_back("Server: " + message);
    }
}

void Chat::Draw(sf::RenderWindow& window) {
    window.draw(m_inputBox);
    window.draw(m_inputText);
    float yOffset = 500;
    for (auto it = m_messages.rbegin(); it != m_messages.rend(); ++it) {
        sf::Text text(*it, m_font, 20);
        text.setFillColor(sf::Color::Black);
        text.setPosition(50, yOffset);
        window.draw(text);
        yOffset -= 25;
        if (yOffset < 50) break;
    }
    std::cout << "draw ³¡" << std::endl;
}

void Chat::SendMessage(sf::TcpSocket& socket) {
    if (!m_currentInput.empty()) {
        sf::Packet packet;
        packet << m_currentInput;

        if (socket.send(packet) == sf::Socket::Done) {
            m_messages.push_back("You: " + m_currentInput);
            m_currentInput.clear();
            m_inputText.setString("");
        }
        else {
            std::cerr << "Error: Could not send message to server" << std::endl;
        }
    }
}
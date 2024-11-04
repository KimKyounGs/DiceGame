#include "Chat.h"
#include <iostream>

Chat::Chat() {
    inputBox.setSize(sf::Vector2f(400, 30));
    inputBox.setPosition(50, 550);
    inputBox.setFillColor(sf::Color::White);

    if (!font.loadFromFile("Arial.ttf")) {
        std::cerr << "Font load error" << std::endl;
    }

    inputText.setFont(font);
    inputText.setCharacterSize(20);
    inputText.setFillColor(sf::Color::Black);
    inputText.setPosition(55, 555);
}

void Chat::HandleEvent(const sf::Event& event, sf::TcpSocket& socket) {
    if (event.type == sf::Event::TextEntered) {
        if (event.text.unicode == '\r') {
            SendMessage(socket);
        }
        else if (event.text.unicode == '\b') {
            if (!currentInput.empty()) {
                currentInput.pop_back();
            }
        }
        else {
            currentInput += static_cast<char>(event.text.unicode);
        }
        inputText.setString(currentInput);
    }
}

void Chat::Update(sf::TcpSocket& socket) {
    sf::Packet packet;
    if (socket.receive(packet) == sf::Socket::Done) {
        std::string message;
        packet >> message;
        messages.push_back("Server: " + message);
    }
}

void Chat::Draw(sf::RenderWindow& window) {
    window.draw(inputBox);
    window.draw(inputText);

    float yOffset = 500;
    for (auto it = messages.rbegin(); it != messages.rend(); ++it) {
        sf::Text text(*it, font, 20);
        text.setFillColor(sf::Color::Black);
        text.setPosition(50, yOffset);
        window.draw(text);
        yOffset -= 25;
        if (yOffset < 50) break;
    }
}

void Chat::SendMessage(sf::TcpSocket& socket) {
    if (!currentInput.empty()) {
        sf::Packet packet;
        packet << currentInput;

        if (socket.send(packet) == sf::Socket::Done) {
            messages.push_back("You: " + currentInput);
            currentInput.clear();
            inputText.setString("");
        }
        else {
            std::cerr << "Error: Could not send message to server" << std::endl;
        }
    }
}
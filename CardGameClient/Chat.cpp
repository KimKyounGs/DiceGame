#include "Chat.h"
#include <iostream>

Chat::Chat() : m_isChatting(false)
{
    m_inputBox.setSize(sf::Vector2f(400, 30));
    m_inputBox.setPosition(50, 550);
    m_inputBox.setFillColor(sf::Color::Cyan);

    if (!m_font.loadFromFile("Font\\RightFont.ttf")) {
        std::cerr << "폰트 로드 실패" << std::endl;
    }

    m_inputText.setFont(m_font);
    m_inputText.setCharacterSize(20);
    m_inputText.setFillColor(sf::Color::Black);
    m_inputText.setPosition(55, 555);
}

Chat::~Chat()
{
    if (m_receivingThread.joinable()) {
        m_receivingThread.join();  // 수신 스레드가 종료될 때까지 대기
    }
}

void Chat::HandleEvent(const sf::Event& event, sf::TcpSocket& socket) 
{
    // O 키를 눌러서 채팅 모드 전환
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::O && !m_isChatting) 
    {
        m_isChatting = !m_isChatting;
        std::cout << "Chat mode " << (m_isChatting ? "활성화" : "비활성화") << std::endl;
    }

    // 채팅 모드가 활성화된 상태에서만 텍스트 입력 처리
    // sf::Event::TextEntered : 키보드로 입력한 텍스트를 감지
    if (m_isChatting && event.type == sf::Event::TextEntered)
    {
        if (event.text.unicode == '\r') // 엔터키로 메시지 전송
        {
            SendMessage(socket);
            m_isChatting = false;
        }
        else if (event.text.unicode == '\b') // 백스페이스
        {
            if (!m_currentInput.empty())
            {
                m_currentInput.pop_back();
            }
        }
        else
        {
            m_currentInput += static_cast<char>(event.text.unicode);
        }
        m_inputText.setString(m_currentInput);
    }
}

void Chat::Update(sf::TcpSocket& socket) 
{
    // 뮤텍스를 잠그고 수신 버퍼의 메시지를 m_messages로 옮김
    std::lock_guard<std::mutex> lock(m_mutex);
    for (const auto& msg : m_receivedMessages) {
        m_messages.push_back(msg);
    }
    m_receivedMessages.clear();
}

void Chat::Draw(sf::RenderWindow& window) 
{
    window.draw(m_inputBox);
    window.draw(m_inputText);
    float yOffset = 500;

    // m_messages 접근 시 잠금
    std::lock_guard<std::mutex> lock(m_mutex);
    for (auto it = m_messages.rbegin(); it != m_messages.rend(); ++it) {
        sf::Text text(*it, m_font, 20);
        text.setFillColor(sf::Color::Black);
        text.setPosition(50, yOffset);
        window.draw(text);
        yOffset -= 25;
        if (yOffset < 50) break;
    }
}
/// <summary>
/// 서버랑 연결되었을 때 한번 호출하여 스레드를 생성하고 이 스레드를 통해서 계속해서 서버에게 수신 받기
/// </summary>
/// <param name="socket"></param>
void Chat::StartReceiving(sf::TcpSocket& socket)
{
    m_receivingThread = std::thread(&Chat::ReceiveMessage, this, std::ref(socket));
}

void Chat::ReceiveMessage(sf::TcpSocket& socket)
{
    while (true)
    {
        sf::Packet packet;
        if (socket.receive(packet) == sf::Socket::Done)
        {
            std::string message;
            packet >> message;

            // 수신 메시지를 뮤텍스로 보호하고 m_receivedMessages에 추가
            std::lock_guard<std::mutex> lock(m_mutex);
            m_receivedMessages.push_back("서버: " + message);
        }
        else
        {
            std::cerr << "Error : 메시지를 수신받을 수 없습니다!" << std::endl;
            break;
        }
    }
}

void Chat::SendMessage(sf::TcpSocket& socket) 
{
    // 메시지를 보낼 때
    if (!m_currentInput.empty()) {
        sf::Packet packet;
        packet << m_currentInput;

        if (socket.send(packet) == sf::Socket::Done) {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_messages.push_back("나: " + m_currentInput);
            m_currentInput.clear();
            m_inputText.setString("");
        }
        else {
            std::cerr << "Error: 서버에 메시지를 송신할 수 없습니다!" << std::endl;
        }
    }
}

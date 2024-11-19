#include "Player.h"
#include <iostream>

Player::Player()
{
    for (int i = 1; i <= 10; i++)
    {
        m_cards[i].SetNum(i);
    }
}

void Player::setTurnChangeCallback(const std::function<void()>& callback) {
    m_turnChangeCallback = callback;  // GameManager에서 콜백 전달
}

void Player::PlayCard()
{
    std::cout << "Player: Card played!" << std::endl;
    // 턴 변경 요청
    if (m_turnChangeCallback) {  // 콜백이 설정되어 있으면 호출
        m_turnChangeCallback();
    }
}

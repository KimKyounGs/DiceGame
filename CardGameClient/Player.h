#pragma once
#ifndef _PlAYER_H_
#define _PlAYER_H_

#include <vector>
#include <string>
#include <functional> // 콜백함수 사용.
#include "Card.h"

class Player
{
private:
	std::string m_playerName;
	std::vector<Card> m_cards;

private:
	Player(const Player&); // 금기
	Player& operator=(const Player&); // 금기

public:
	Player(); // 생성자
	void PlayCard(); // 카드를 내는 함수
	
};


#endif // !_PlAYER_H_
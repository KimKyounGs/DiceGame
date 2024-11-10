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

public:
	Player();


};


#endif // !_PlAYER_H_
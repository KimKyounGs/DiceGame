#pragma once
#ifndef _GAMEMANAGER_H
#define _GAMEMANAGER_H

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "Card.h"

class GameManager 
{
private:
	Card m_playerCards[10];
	Card m_enemyCards[10];
	bool m_isMyTurn;

private:
	GameManager(const GameManager&);
	~GameManager();

public:
	GameManager();
	
	void GameStart(); // 게임 시작할 때 함수
	void TurnChange(); // 턴 변경

};


#endif // !_GAMEMANAGER_H
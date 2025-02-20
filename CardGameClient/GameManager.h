#pragma once
#ifndef _GAMEMANAGER_H
#define _GAMEMANAGER_H

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <functional>
#include "MainGame.h"
#include "Player.h"


class GameManager 
{
private:
	Player m_player; // 플레이어
	bool m_isMyTurn; // 턴 유무

private:
	GameManager(const GameManager&); // 사용 안하는 함수 금하기
	GameManager& operator=(const GameManager&); // 사용 안하는 함수 금하기

public:
	GameManager();
	
	void GameStart(); // 게임 시작할 때 함수
	void ChangeTurn(); // 턴 변경 함수를 콜백 준비
};


#endif // !_GAMEMANAGER_H
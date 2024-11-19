#include "GameManager.h"
#include "iostream"

GameManager::GameManager() : m_isMyTurn(false)
{
	m_player.setTurnChangeCallback(std::bind(&GameManager::ChangeTurn, this));
}

void GameManager::GameStart()
{
	// 송신하기 -> 턴 정해주기. m_isMyTurn = 함수
	std::cout << "게임 시작!" << std::endl;
	
}

void GameManager::ChangeTurn()
{
	m_isMyTurn = !m_isMyTurn;
	if (m_isMyTurn)
	{
		std::cout << "내턴!" << std::endl;
	}
	else
	{
		std::cout << "상대턴!" << std::endl;
	}

}
#include "GameManager.h"
#include "iostream"

GameManager::GameManager() : m_player(m_player),m_isMyTurn(false)
{
	for (int i = 1; i <= 10; i++)
	{
		(*m_playerCards)->SetNum(i);
		(*m_enemyCards)->SetNum(i);
	}
	m_player.setTurnChangeCallback(std::bind(&GameManager::ChangeTurn, this));
}

void GameManager::ChangeTurn()
{
	std::cout << "GameManager: Turn has been changed!" << std::endl;
}


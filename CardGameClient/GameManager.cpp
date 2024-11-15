#include "GameManager.h"

GameManager::GameManager() : m_isMyTurn(false)
{
	for (int i = 1; i <= 10; i++)
	{
		(*m_playerCards)->SetNum(i);
		(*m_enemyCards)->SetNum(i);
	}
}

void GameManager::TurnChange()
{

}

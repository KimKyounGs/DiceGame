#include "Card.h"

Card::Card() : m_cardNum(0), m_cardParity(Parity::Defalut), m_isUsed(false) {};


void Card::SetNum(const int& num)
{
	m_cardNum = num;
}

const int Card::GetNum()
{
	return m_cardNum;
}

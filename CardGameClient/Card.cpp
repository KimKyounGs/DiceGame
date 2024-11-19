#include "Card.h"

Card::Card() : m_cardNum(0), m_cardParity(Parity::Defalut), m_isUsed(false) {};


void Card::SetNum(const int& num)
{
	m_cardNum = num;
	if (m_cardNum % 2 == 0) m_cardParity = Parity::Even;	// Â¦¼ö
	else m_cardParity = Parity::Odd;	// È¦¼ö
}

const int Card::GetNum()
{
	return m_cardNum;
}

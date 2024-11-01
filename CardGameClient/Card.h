#ifndef _CARD_H_ // 헤더가드 로 인해서 Card.h는 한 번만 포함되기 때문에 중복 에러 발생을 방지 
#define _CARD_H_

/// <summary>
/// include
/// </summary>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>

/// <summary>
/// Class Name = Card
/// </summary>
class Card {

public:
	Card();
	Card(const Card&);
	~Card();

	// 프로퍼티 함수

private:
	int _cardId;

private:


};








#endif // !_CARD_H_
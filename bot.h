#include "physics.h"
#pragma once
class bot: public physics
{
public:
	//functions
	bot(sf::RenderWindow& win, sf::Sprite& temp, sf::Texture img) :physics(win, temp, img) {}
	void BotBehaviour();
	void WalkAnimation(int direction);
	void movement();
	//varables
};


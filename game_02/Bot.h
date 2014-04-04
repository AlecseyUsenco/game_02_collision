#pragma once
#include "Car.h"
class Bot :
	public Car
{
public:
	vector3df bot_roat, bot_position;
	void BotPosition(float dx, float dy, float dz);
	void Show();
	void Move(float dx, float dy, float dz);
	Bot();
	~Bot();
};


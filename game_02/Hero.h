#pragma once
#include "Car.h"
class Hero :
	public Car
{
public:
	vector3df hero_position, hero_roat;
	void HeroPosition(float dx, float dy, float dz);
	void Show();
	void Move(float dx, float dy, float dz, int textura, vector3df scale);
	Hero();
	~Hero();
};


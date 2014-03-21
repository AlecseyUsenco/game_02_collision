#include<irrlicht.h>
#include <string>
#include <iostream>
#include <map>
#pragma once
using namespace irr;
using namespace core;
using namespace scene;
class Car
{
public:
	ISceneNode *node;
	vector3df car_position;
	vector3df hero_position, hero_roat;
	void Create(int models, vector3df scale);
	void SetPosition(int dx, int dy, int dz);
	void HeroPosition(float dx, float dy, float dz);
	void Show_Enemy(int LongRoad_X, int LongRoad_Y, int LongRoad_Z, int sped, int roat);
	void Show_Player();
	void Hide();
	void Move(float dx, float dy, float dz);
	Car(void);
	~Car(void);
};


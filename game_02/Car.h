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
	void Create(int models, vector3df scale);
	void Hide();
	Car(void);
	~Car(void);
};


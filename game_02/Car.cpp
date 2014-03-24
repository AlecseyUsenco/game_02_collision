#include "Car.h"

extern ISceneManager *smgr;

Car::Car(void)
{
	car_position=vector3df(4000,10,-200);
	hero_position=vector3df(10000,100,300);
	hero_roat=vector3df(90,-140,0);
}

void Car::Create(int models, vector3df scale)
{
	std::map<int,core::string<char>>m;
	m[0]="../models/car_easy/Car_chrysler.3ds";
	m[1]="../models/car_4/Mitsubishi_Fuso.3ds";
	m[2]="../models/car_enemy/Car N310309.3ds";
	m[3]="../models/car_police/skoda-octavia.3ds";
	m[4]="../models/car_police/POLICIE.3ds";
	m[5]="../models/car_1/Car aston martin v12 vantage 2010 N220513.3ds";
	m[6]="../models/Car Lamborghini Gallardo N291012.3ds";
	m[7]="../models/car_1/Ferrari.3ds";
	m[8]="../models/car_2/Jaguar.3ds";
	m[9]="../models/car_3/NISSAN.3ds";
	core::string<char> f = m[models];
	IAnimatedMesh *mesh=smgr->getMesh(f);
	node=smgr->addAnimatedMeshSceneNode(mesh);
	node->setScale(vector3df(scale.X,scale.Y,scale.Z));
	node->setMaterialFlag(video::EMF_LIGHTING, false);
}

void Car::SetPosition(int dx, int dy, int dz)
{
	car_position=vector3df(this->car_position.X+dx,this->car_position.Y+dy,this->car_position.Z+dz);
}

void Car::HeroPosition(float dx, float dy, float dz)
{
	hero_position=vector3df(this->hero_position.X+dx,this->hero_position.Y+dy,this->hero_position.Z+dz);
}

void Car::Hide()
{
	node->setVisible(false);
	node=NULL;
}

void Car::Show_Enemy(int LongRoad_X, int LongRoad_Y, int LongRoad_Z, int sped, int roat)
{
	node->setRotation(vector3df(0,roat,0));
	node->setPosition(car_position);
	node->setVisible(true);
	ISceneNodeAnimator* anim = smgr->createFlyStraightAnimator(core::vector3df(car_position.X,car_position.Y,car_position.Z), core::vector3df(car_position.X+LongRoad_X,car_position.Y+LongRoad_Y,car_position.Z+LongRoad_Z), sped, true);
	if (anim)
	{
	node->addAnimator(anim);
	anim->drop();
	}
}

void Car::Show_Player()
{
	node->setRotation(hero_roat);
	node->setPosition(hero_position);
	node->setVisible(true);
}

void Car::Move(float dx, float dy, float dz)
{
	Hide();
	Create(2, vector3df(0.006, 0.005, 0.005));
	HeroPosition(dx, dy, dz);
	Show_Player();
}

Car::~Car(void)
{
}

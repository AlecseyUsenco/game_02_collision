 #include "Car.h"

extern ISceneManager *smgr;

Car::Car(void)
{
}

Car::~Car(void)
{
}

void Car::Create(int models, vector3df scale)
{
	std::map<int,core::string<char>>m;
	m[0]="../models/car_easy/Car_chrysler.3ds";
	m[1]="../models/car_4/Mitsubishi_Fuso.3ds";
	m[2]="../models/car_enemy/Car N310309.3ds";
	m[3]="../models/car_police/skoda-octavia.3ds";
	m[4]="../models/car_police/POLICIE.3ds";
	m[7]="../models/car_1/Ferrari.3ds";
	m[8]="../models/car_2/Jaguar.3ds";
	m[9]="../models/car_3/NISSAN.3ds";
	m[10]="../models/ford/Dodge.3ds";
	m[11]="../models/bmw.3DS";
	m[12] = "../models/audi/r8.3DS";
	core::string<char> f = m[models];
	IAnimatedMesh *mesh=smgr->getMesh(f);
	node=smgr->addAnimatedMeshSceneNode(mesh);
	node->setScale(vector3df(scale.X,scale.Y,scale.Z));
	node->setMaterialFlag(video::EMF_LIGHTING, false);
}

void Car::Hide()
{
	node->setVisible(false);
	node=NULL;
}

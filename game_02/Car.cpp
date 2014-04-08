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
	m[1] = "../models/bmw/bmw.3DS";
	m[3] = "../models/audi/r8.3DS";
	m[8] = "../models/audi_2/rs4.3DS";
	m[9] = "../models/aston_martin/db9.3DS";
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

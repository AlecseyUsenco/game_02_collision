#include<iostream>
#include<irrlicht.h>
#include<irrKlang.h>

#include "driverChoice.h"
#include "Car.h"
#include "Globals.h"
#include "MyEventReceiver.h"

using namespace irr;
using namespace scene;
using namespace core;
using namespace video;
using namespace gui;
using namespace io;
using namespace irrklang;
using namespace std;

enum
{
	ID_IsNotPickable = 0,
	IDFlag_IsPickable = 1 << 0,
	IDFlag_IsHighlightable = 1 << 1
};

#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#pragma comment(lib, "irrKlang.lib")
#endif

extern int width=1366, height=768;
extern IrrlichtDevice* device=createDevice(EDT_DIRECT3D9, dimension2d<u32>(width, height), 16, false, false, false, 0);
extern ISceneManager* smgr=device->getSceneManager();
extern IVideoDriver* driver=device->getVideoDriver();

int main()
{
	// Тестовое изминение для ГитХаба

	////////////////////////////////////////////////////////////////////
	int menu=0;
	IGUIEnvironment *gui = device->getGUIEnvironment();

	ISoundEngine* engine = createIrrKlangDevice();
	ISoundEngine* dvigatel = createIrrKlangDevice();
	ISoundEngine* tormoz = createIrrKlangDevice();

	engine->play2D("../audio/Undeground.mp3", true);
	SIrrlichtCreationParameters deviceParam;
	deviceParam.WindowSize.Width;
	deviceParam.WindowSize.Height;

	IGUIComboBox *VideoMode = gui->addComboBox(rect<s32>( 550, 359, 760, 376 ), 0, GUI_ID_VIDEO_MODE);
	gui->clear();
	IVideoModeList *modeList = device->getVideoModeList();
	IGUIScrollBar* Gamma;

	SAppContext context_game;
	context_game.device = device;
	context_game.gui=gui;
	context_game.menu=menu;
	context_game.Width=width;
	context_game.Height=height;
	context_game.VideoMode=VideoMode;
	MyEventReceiver receiver(context_game);
	device->setEventReceiver(&receiver);
	
	//////////////////////////////////////////////////////////////////////
	while(context_game.menu!=100)
	{
		if(context_game.menu==12)
		{
			engine->stopAllSounds();
			context_game.menu=0;
			device=createDevice(EDT_DIRECT3D9, dimension2d<u32>(width, height), 16, true, false, false, &receiver);
			smgr=device->getSceneManager();
			gui=device->getGUIEnvironment();
			driver=device->getVideoDriver();
			engine->play2D("../audio/Undeground.mp3", true);
		}

		context_game.device = device;
		context_game.gui=gui;
		context_game.menu=menu;
		context_game.Width=width;
		context_game.Height=height;
		context_game.VideoMode=VideoMode;
		MyEventReceiver receiver(context_game);
		device->setEventReceiver(&receiver);

		int speadXCeloe=0, Old_roat=0, read=0, start=0, total_start, total_vin, start_sprint, vin_sprint, start_drag, vin_drag, new_game=1, SetIdent=0;
		float roat=90, i=0, speed=0, spead_old=0, MoveX=-1, MoveZ=0, RXA=621, RZA=0, lookat_cameraOld=0;
		char buf[256];
		f32 GammaValue=1.f;
		vector3df position_camera=vector3df(10721,220,300);
		vector3df lookat_camera=vector3df(1000,300,300);
		ICameraSceneNode* camera = smgr->addCameraSceneNode(0,position_camera,lookat_camera);

		IAnimatedMesh* mesh=smgr->getMesh("../models/Track_12.3ds");
		IAnimatedMesh* garage=smgr->getMesh("../models/Garage.3ds");
		IMeshSceneNode *node = 0, *room = 0;
		video::SMaterial material;

		node=smgr->addMeshSceneNode(mesh);
		room=smgr->addMeshSceneNode(garage);

		if (mesh)
		{
			node = smgr->addOctreeSceneNode(mesh->getMesh(0), 0, IDFlag_IsPickable);
		}
		scene::ITriangleSelector* selector = 0;
		if (node)
		{

			selector = smgr->createOctreeTriangleSelector(node->getMesh(), node, 128);
			node->setTriangleSelector(selector);
		}
		if (selector)
		{
			scene::ISceneNodeAnimator* anim = smgr->createCollisionResponseAnimator(
			selector, camera, core::vector3df(30,50,30),
			core::vector3df(0,-10,0), core::vector3df(0,30,0));
			selector->drop(); // As soon as we're done with the selector, drop it.
			camera->addAnimator(anim);
			anim->drop();  // And likewise, drop the animator when we're done referring to it.
		}

			scene::IBillboardSceneNode * bill = smgr->addBillboardSceneNode();
			bill->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR );
			bill->setMaterialTexture(0, driver->getTexture("../models/particle.bmp"));
			bill->setMaterialFlag(video::EMF_LIGHTING, false);
			bill->setMaterialFlag(video::EMF_ZBUFFER, false);
			bill->setSize(core::dimension2d<f32>(20.0f, 20.0f));
			bill->setID(ID_IsNotPickable);

		scene::ISceneNode* highlightedSceneNode = 0;
		scene::ISceneCollisionManager* collMan = smgr->getSceneCollisionManager();
		material.Wireframe=true;

		node->setRotation(vector3df(0,90,0));
		room->setRotation(vector3df(0,-90,0));
		node->setScale(core::vector3df(30,23,30));
		room->setScale(core::vector3df(11,11,11));
		node->setMaterialFlag(video::EMF_LIGHTING, false);
		room->setMaterialFlag(video::EMF_LIGHTING, false);
		room->setPosition(vector3df(10000,0,300));
		room->setVisible(true);
		node->setVisible(false);
		node->setID(IDFlag_IsPickable);
		ISceneNode* skydome=smgr->addSkyDomeSceneNode(driver->getTexture("../models/skydome.jpg"),16,8,0.95f,2.0f);
		driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, true);

		gui = device->getGUIEnvironment();
		IGUISkin* skin = gui->getSkin();
		IGUIFont* font = gui->getFont("../models/fonthaettenschweiler.bmp");
		if (font) skin->setFont(font);

		Car streat_car[2];
		streat_car[0].Create(0, vector3df(1.45,1.45,1.45)); // Ferrari 0.2,0.15,0.175
		streat_car[0].Show_Enemy(-1600,0,0,5000,0);
		Car Hero;
		Hero.Create(2, vector3df(0.006,0.005,0.005));
		Hero.Show_Player();
		Hero.node->setID(IDFlag_IsPickable);

		while(device->run())
		{
			if(device->isWindowActive())
			{
			ICameraSceneNode* camera = smgr->addCameraSceneNode(0,position_camera,lookat_camera);
			camera->setFarValue(30000);

			// Главное меню
			if(context_game.menu==0)
			{
				driver->beginScene(true, true, SColor(255,255,255,255));
				gui->addButton(rect< s32 >(10,254,200,296), 0, GUI_ID_KARER, L"Karer");
				gui->addButton(rect< s32 >(10,306,200,348), 0, GUI_ID_FAST_START, L"Fast Start");
				gui->addButton(rect<s32>(10,360,200,402), 0, GUI_ID_STATISTIC, L"Statistic");
				gui->addButton(rect< s32 >(10,414,200,456), 0, GUI_ID_OPTIONS, L"Options");
				gui->addButton(rect< s32 >(10,468,200,510), 0, GUI_ID_QUIT, L"Quit");
				smgr->drawAll();
				gui->drawAll();
				driver->endScene();
			}
			// Карьера
			if(context_game.menu==1)
			{
				driver->beginScene(true, true, SColor(255,255,255,255));
				gui->addButton(rect< s32 >(10,252,200,296), 0, GUI_ID_START, L"Start");
				gui->addButton(rect< s32 >(10,306,200,348), 0, GUI_ID_GARAGE, L"Garage");
				gui->addButton(rect< s32 >(10,360,200,402), 0, GUI_ID_KARER, L"Save");
				gui->addButton(rect< s32 >(0,726,170,768), 0, GUI_ID_BACK, L"Back");
				smgr->drawAll();
				gui->drawAll();
				driver->endScene();
			}
			// Гараж
			if(context_game.menu==5)
			{
				driver->beginScene(true, true, SColor(255,255,255,255));
				gui->addButton(rect< s32 >(10,252,200,296), 0, GUI_ID_TUNING, L"Tuning");
				gui->addButton(rect< s32 >(10,306,200,348), 0, GUI_ID_STAILING, L"Stailing");
				gui->addButton(rect< s32 >(10,360,200,402), 0, GUI_ID_CAR, L"Car");
				gui->addButton(rect< s32 >(0,726,170,768), 0, GUI_ID_BACK_KARER, L"Back");
				smgr->drawAll();
				gui->drawAll();
				driver->endScene();
			}
			// Тюнинг
			if(context_game.menu==6)
			{
				driver->beginScene(true, true, SColor(255,255,255,255));
				gui->addButton(rect< s32 >(0,726,170,768), 0, GUI_ID_BACK_GARAGE, L"Back");
				smgr->drawAll();
				gui->drawAll();
				driver->endScene();
			}
			// Стайлинг
			if(context_game.menu==7)
			{
				driver->beginScene(true, true, SColor(255,255,255,255));
				gui->addButton(rect< s32 >(0,726,170,768), 0, GUI_ID_BACK_GARAGE, L"Back");
				smgr->drawAll();
				gui->drawAll();
				driver->endScene();
			}
			// Машины
			if(context_game.menu==8)
			{
				driver->beginScene(true, true, SColor(255,255,255,255));
				gui->addButton(rect< s32 >(0,726,170,768), 0, GUI_ID_BACK_GARAGE, L"Back");
				gui->addButton(rect< s32 >(600,691,670,733), 0, GUI_ID_LAST_CAR, L"Last");
				gui->addButton(rect< s32 >(730,691,800,733), 0, GUI_ID_NEXT_CAR, L"Next");
				smgr->drawAll();
				gui->drawAll();
				driver->endScene();
			}
			// Статистика
			if(context_game.menu==3)
			{
				if(read==0)
				{
					FILE *write=fopen("../models/stat.txt","rt");
					fscanf(write,"%d%d%d%d%d%d", &total_start, &total_vin, &start_sprint, &vin_sprint, &start_drag, &vin_drag);
					fclose(write);
					read=1;
				}
				stringw Total_start(total_start), Total_vin(total_vin), Start_sprint(start_sprint), Vin_sprint(vin_sprint), Start_drag(start_drag), Vin_drag(vin_drag);
				driver->beginScene(true, true, SColor(255,255,255,255));
				gui->addStaticText(L"   total start  : ",rect<s32>(515,200,600,235), false , true, (IGUIElement*)0, -1, true);
				gui->addStaticText(Total_start.c_str(),rect<s32>(600,200,665,235), false , true, (IGUIElement*)0, -1, true);
				gui->addStaticText(L"   total vin    : ",rect<s32>(515,250,600,285), false , true, (IGUIElement*)0, -1, true);
				gui->addStaticText(Total_vin.c_str(),rect<s32>(600,250,665,285), false , true, (IGUIElement*)0, -1, true);
				gui->addStaticText(L"   start sprint : ",rect<s32>(515,300,600,335), false , true, (IGUIElement*)0, -1, true);
				gui->addStaticText(Start_sprint.c_str(),rect<s32>(600,300,665,335), false , true, (IGUIElement*)0, -1, true);
				gui->addStaticText(L"   vin sprint   : ",rect<s32>(515,350,600,385), false , true, (IGUIElement*)0, -1, true);
				gui->addStaticText(Vin_sprint.c_str(),rect<s32>(600,350,665,385), false , true, (IGUIElement*)0, -1, true);
				gui->addStaticText(L"   start drag   : ",rect<s32>(515,400,600,435), false , true, (IGUIElement*)0, -1, true);
				gui->addStaticText(Start_drag.c_str(),rect<s32>(600,400,665,435), false , true, (IGUIElement*)0, -1, true);
				gui->addStaticText(L"   vin drag     : ",rect<s32>(515,450,600,485), false , true, (IGUIElement*)0, -1, true);
				gui->addStaticText(Vin_drag.c_str(),rect<s32>(600,450,665,485), false , true, (IGUIElement*)0, -1, true);
				gui->addButton(rect< s32 >(0,726,170,768), 0, GUI_ID_BACK, L"Back");
				smgr->drawAll();
				gui->drawAll();
				driver->endScene();
			}
			// Опции
			if(context_game.menu==4)
			{
				driver->beginScene(true, true, SColor(255,255,255,255));
				gui->addStaticText(L"   Sound Volume",rect<s32>(450,200,535,235), false , true, (IGUIElement*)0, -1, true);
				IGUIScrollBar* scrollbar1 = gui->addScrollBar(true,rect<s32>(550,209,760,226), 0, GUI_ID_TRANSPARENCY_SCROLL_BAR);
				scrollbar1->setMax(255);
				scrollbar1->setPos(gui->getSkin()->getColor(EGDC_WINDOW).getAlpha());
				gui->addStaticText(L"   Efect Volume",rect<s32>(450,250,535,285), false , true, (IGUIElement*)0, -1, true);
				IGUIScrollBar* scrollbar2 = gui->addScrollBar(true,rect<s32>(550,259,760,276), 0, GUI_ID_TRANSPARENCY_SCROLL_BAR);
				scrollbar2->setMax(255);
				scrollbar2->setPos(gui->getSkin()->getColor(EGDC_WINDOW).getAlpha());
				gui->addButton(rect< s32 >(0,726,170,768), 0, GUI_ID_BACK, L"Back");

				gui->addStaticText(L"   Video Mode",rect<s32>(450,350,535,385), false , true, (IGUIElement*)0, -1, true);
				VideoMode = gui->addComboBox(rect<s32>( 550, 359, 760, 376 ), 0, GUI_ID_VIDEO_MODE);
				if(modeList)
				{
					s32 i;
					for(i=0; i!=modeList->getVideoModeCount(); ++i)
					{
						u16 d = modeList->getVideoModeDepth(i);
						if(d<16)
						continue;

						u16 w = modeList->getVideoModeResolution ( i ).Width;
						u16 h = modeList->getVideoModeResolution ( i ).Height;
						u32 val = w << 16 | h;

						if ( VideoMode->getIndexForItemData ( val ) >= 0 )
						continue;

						f32 aspect = (f32) w / (f32) h;
						const c8 *a = "";
						if ( core::equals ( aspect, 1.3333333333f ) ) a = "4:3";
						else if ( core::equals ( aspect, 1.6666666f ) ) a = "15:9 widescreen";
						else if ( core::equals ( aspect, 1.7777777f ) ) a = "16:9 widescreen";
						else if ( core::equals ( aspect, 1.6f ) ) a = "16:10 widescreen";
						else if ( core::equals ( aspect, 2.133333f ) ) a = "20:9 widescreen";
						snprintf ( buf, sizeof ( buf ), "%d x %d, %s",w, h, a );
						VideoMode->addItem ( stringw ( buf ).c_str(), val ); // receiver.Context.
					}
				}
			
				VideoMode->setSelected ( VideoMode->getIndexForItemData ( deviceParam.WindowSize.Width << 16 | deviceParam.WindowSize.Height ) );
				receiver.Context.VideoMode = VideoMode;

				gui->addButton(rect< s32 >(780,357,820,375), 0, GUI_ID_SET, L"Set");

				gui->addStaticText ( L"   Gamma:", rect<s32>(450,300,535,335), false, true, 0, -1, true );
				Gamma = gui->addScrollBar( true, rect<s32>( 550, 311, 760, 326 ), 0,-1 );
				Gamma->setMin ( 50 );
				Gamma->setMax ( 350 );
				Gamma->setSmallStep ( 1 );
				Gamma->setLargeStep ( 10 );
				Gamma->setPos ( core::floor32 ( GammaValue * 100.f ) );
				device->setGammaRamp ( GammaValue, GammaValue, GammaValue, 0.f, 0.f );

				smgr->drawAll();
				gui->drawAll();
				driver->endScene();
			}

			// Разрешение
			if(context_game.menu==12)
			{
				//context_game.menu=0;
				device->closeDevice();
				/*device=createDevice(EDT_DIRECT3D9, dimension2d<u32>(width, height), 16, true, false, false, &receiver);
				smgr=device->getSceneManager();
				driver=device->getVideoDriver();
				cout<<context_game.menu<<endl;*/
			}

			// Игра
			if(context_game.menu==2)
			{

				if (highlightedSceneNode)
				{
					highlightedSceneNode->setMaterialFlag(video::EMF_LIGHTING, true);
					highlightedSceneNode = 0;
				}
				core::line3d<f32>ray;
				ray.start = camera->getPosition();
				ray.end = ray.start + (camera->getTarget() - ray.start).normalize() * 1000.0f;

				core::vector3df intersection;
				core::triangle3df hitTriangle;

				scene::ISceneNode * selectedSceneNode =
				collMan->getSceneNodeAndCollisionPointFromRay( ray,
				intersection, // точка столкновения
				hitTriangle, // полигон(треугольник) в котором точка столкновения
				IDFlag_IsPickable, // определять столкновения только для нод с идентификатором IDFlag_IsPickable
				0); 

				if(selectedSceneNode)
				{
					bill->setPosition(intersection); // Hero.node
					Hero.node->setPosition(intersection);
					driver->setTransform(video::ETS_WORLD, core::matrix4());
					driver->setMaterial(material);
					driver->draw3DTriangle(hitTriangle, video::SColor(0,255,0,0));

					if((selectedSceneNode->getID() & IDFlag_IsHighlightable) == IDFlag_IsHighlightable)
					{
						highlightedSceneNode = selectedSceneNode;

						highlightedSceneNode->setMaterialFlag(video::EMF_LIGHTING, false);
					}
				}

				if(new_game==1)
				{
					engine->stopAllSounds();
					engine->play2D("../audio/Static_X.mp3");
					read=0;
					room->setVisible(false);
					node->setVisible(true);
					Hero.hero_roat=vector3df(90,90,0);
					position_camera=vector3df(10621,300,300);
					new_game=0;
				}

				Hero.Move(speed*MoveX,0,speed*MoveZ);
				lookat_camera.X+=MoveX*speed;
				lookat_camera.Z+=MoveZ*speed;
				camera->setPosition(vector3df(position_camera.X+=speed*MoveX,position_camera.Y,position_camera.Z+=speed*MoveZ));
				if(speed<=0)
				{
					dvigatel->stopAllSounds(); 
					gui->addStaticText(L"	0",rect<s32>(125,670,175,710), false , true, (IGUIElement*)0, -1, true);
				}

				if(speed>0)
				{
					speed-=0.005;
					speadXCeloe=5*speed;
					gui->clear();
					stringw Speed(speadXCeloe);
					gui->addStaticText(Speed.c_str(),rect<s32>(125,670,175,710), false , true, (IGUIElement*)0, -1, true);
				}

				//Кнопки
				if(receiver.IsKeyDown(irr::KEY_KEY_W))
				{
					if(speed<10)
					{ 
						speed+=0.03; 
						if(speed<=0.03)
						{
							dvigatel->play2D("../audio/dvigatel.mp3",true);
						}
					}
					if(speed>=10 && speed<18) { speed+=0.015; }
					if(speed>=18 && speed<25) {	speed+=0.01; }
				}

				if(receiver.IsKeyDown(irr::KEY_KEY_S))
				{
					speed-=0.09;
					tormoz->stopAllSounds();
					tormoz->play2D("../audio/Tormozov.mp3");
				}

				if(receiver.IsKeyDown(irr::KEY_KEY_A))
				{
					roat-=1;
					Hero.node->setRotation(vector3df(93,roat,0)); // поворот модели на месте
					Hero.hero_roat.Y-=1; // для поворота при движении
					if(Hero.hero_roat.Y==-271)
					{
						Hero.hero_roat.Y=90;
						roat=90;
						Old_roat=90;
					}
					if(Hero.hero_roat.Y<90 && Hero.hero_roat.Y>=0)
					{
						MoveX+=0.011;
						MoveZ-=0.011;
						if(MoveX>-0.025 && MoveZ<-0.95)
						{
							MoveX=0;
							MoveZ=-1;
						}
						RZA+=6.9; // Rotation Z key A
						RXA-=6.9;
						position_camera.X=Hero.hero_position.X+RXA;
						position_camera.Z=Hero.hero_position.Z+RZA;
						lookat_camera.X+=100;
						lookat_camera.Z-=100;
					}
					if(Hero.hero_roat.Y<0 && Hero.hero_roat.Y>=-90 || Hero.hero_roat.Y<=360 && Hero.hero_roat.Y>270)
					{
						if(MoveX<0.95 && MoveZ<-0.05)
						{
							MoveX+=0.011;
							MoveZ+=0.011;
						}
						if(MoveX>0.95 && MoveZ>-0.05)
						{
							MoveX=1;
							MoveZ=0;
						}
						RZA-=6.9;
						RXA-=6.9;
						position_camera.X=Hero.hero_position.X+RXA;
						position_camera.Z=Hero.hero_position.Z+RZA;
						lookat_camera.X+=100;
						lookat_camera.Z+=100;
					}
					if(Hero.hero_roat.Y<-90 && Hero.hero_roat.Y>=-180 || Hero.hero_roat.Y<=270 && Hero.hero_roat.Y>=180)
					{
						if(MoveX<0.05 && MoveZ>0.95)
						{
							MoveX=0;
							MoveZ=1;
						}
						if(MoveX>0.05 && MoveZ<0.95)
						{
							MoveX-=0.011;
							MoveZ+=0.011;
						}
						RZA-=6.9;
						RXA+=6.9;
						position_camera.X=Hero.hero_position.X+RXA;
						position_camera.Z=Hero.hero_position.Z+RZA;
						lookat_camera.X-=100;
						lookat_camera.Z+=100;
					}
					if(Hero.hero_roat.Y<-180 && Hero.hero_roat.Y>=-270 || Hero.hero_roat.Y<180 && Hero.hero_roat.Y>=90)
					{
						if(MoveX<-0.95 && MoveZ<0.05)
						{
							MoveX=-1;
							MoveZ=0;
						}
						if(MoveX>-0.95 && MoveZ>0.05)
						{
							MoveX-=0.011;
							MoveZ-=0.011;
						}
						RZA+=6.9;
						RXA+=6.9;
						position_camera.X=Hero.hero_position.X+RXA;
						position_camera.Z=Hero.hero_position.Z+RZA;
						lookat_camera.X-=100;
						lookat_camera.Z-=100;
					}
				}

				if(receiver.IsKeyDown(irr::KEY_KEY_D))
				{
					roat+=1;
					Hero.node->setRotation(vector3df(93,roat,0)); // поворот модели на месте
					Hero.hero_roat.Y+=1; // для поворота при движении
					if(Hero.hero_roat.Y==360)
					{
						Hero.hero_roat.Y=0;
						roat=0;
						Old_roat=0;
					}
					if(Hero.hero_roat.Y>90 && Hero.hero_roat.Y<=180 || Hero.hero_roat.Y>-270 && Hero.hero_roat.Y<=-180)
					{
						if(MoveX>-0.05 && MoveZ>0.95)
						{
							MoveX=0;
							MoveZ=1;
						}
						if(MoveX<-0.05 && MoveZ<0.95)
						{
							MoveX+=0.011;
							MoveZ+=0.011;
						}
						RZA-=6.9; // Rotation Z key A
						RXA-=6.9;
						position_camera.X=Hero.hero_position.X+RXA;
						position_camera.Z=Hero.hero_position.Z+RZA;
						lookat_camera.X+=100;
						lookat_camera.Z+=100;
					}
					if(Hero.hero_roat.Y>180 && Hero.hero_roat.Y<=270 || Hero.hero_roat.Y>-180 && Hero.hero_roat.Y<=-90)
					{
						if(MoveX>0.95 && MoveZ<0.05)
						{
							MoveX=1;
							MoveZ=0;
						}
						if(MoveX<0.95 && MoveZ>0.05)
						{
							MoveX+=0.011;
							MoveZ-=0.011;
						}
						RZA+=6.9;
						RXA-=6.9;
						position_camera.X=Hero.hero_position.X+RXA;
						position_camera.Z=Hero.hero_position.Z+RZA;
						lookat_camera.X+=100;
						lookat_camera.Z-=100;
					}
					if(Hero.hero_roat.Y>270 && Hero.hero_roat.Y<=360 || Hero.hero_roat.Y>-90 && Hero.hero_roat.Y<=0)
					{
						if(MoveX<0.05 && MoveZ<-0.95)
						{
							MoveX=0;
							MoveZ=-1;
						}
						if(MoveX>0.05 && MoveZ>-0.95)
						{
							MoveX-=0.011;
							MoveZ-=0.011;
						}
						RZA+=6.9;
						RXA+=6.9;
						position_camera.X=Hero.hero_position.X+RXA;
						position_camera.Z=Hero.hero_position.Z+RZA;
						lookat_camera.X-=100;
						lookat_camera.Z-=100;
					}
					if(Hero.hero_roat.Y>0 && Hero.hero_roat.Y<=90)
					{
						if(MoveX<-0.95 && MoveZ>-0.05)
						{
							MoveX=-1;
							MoveZ=0;
						}
						if(MoveX>-0.95 && MoveZ<-0.05)
						{
							MoveX-=0.011;
							MoveZ+=0.011;
						}
						RZA-=6.9;
						RXA+=6.9;
						position_camera.X=Hero.hero_position.X+RXA;
						position_camera.Z=Hero.hero_position.Z+RZA;
						lookat_camera.X-=100;
						lookat_camera.Z+=100;
					}
				}

				driver->beginScene(true, true, SColor(255,255,255,255));
				smgr->drawAll();
				device->getGUIEnvironment()->drawAll();
				driver->endScene();
			}
			}
		}
	}
	device->drop();
	return 0;
}

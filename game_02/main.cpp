#include<iostream>
#include<irrlicht.h>
#include<irrKlang.h>

#include "MyEventReceiver.h"
#include "Globals.h"
#include "Hero.h"
#include "Bot.h"

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
extern IrrlichtDevice* device=createDevice(EDT_DIRECT3D9, dimension2d<u32>(width, height), 32, true, false, false, 0);
extern ISceneManager* smgr=device->getSceneManager();
extern IVideoDriver* driver=device->getVideoDriver();

int main()
{
	////////////////////////////////////////////////////////////////////
	int garageCreate=0, money = 15000, cena=40000, sounds = 0, bot_delete = 0, choise_track = 0, car = 1, textura = 9, timeCeloe = 50, menu = 0, sound = 1, return_game = 0, lastFPS = -1, fps, OptionsCreate = 0, win = 0, Bot_posX[31], Bot_posZ[31], Bot_roat[31];
	int carChoise=1, car_old=0, speadXCeloe=0, Old_roat=0, read=0, start=0, total_start, total_vin, start_sprint, vin_sprint, start_drag, vin_drag, new_game=1, SetIdent=0, num=0, num_sled=0, delitel=0;
	float time, roat = 90, i = 0, speed = 0, spead_old = 0, MoveX = -1, MoveZ = 0, RXA = 621, RZA = 0, lookat_cameraOld = 0, bot_MoveX = 0, bot_MoveZ = 0, bot_roat = 90;
	char buf[256];
	line3d<f32>ray;
	vector3df position_camera, lookat_camera, intersection, scale_start(150, 150, 150), roat_start(0, -135, 0), position_start(9950, 0, 300);
	triangle3df hitTriangle;
	f32 GammaValue=1.f;
	Hero Hero;
	Bot bot;
	Hero.Create(9, vector3df(150, 150, 150)); // 2, vector3df(0.006, 0.005, 0.005)
	aabbox3d<float> bboxHero;
	aabbox3d<float> bboxStreetCar;
	ISceneNode *selectedSceneNode;
	IGUIEnvironment *gui = device->getGUIEnvironment();
	IGUIEnvironment *env = device->getGUIEnvironment();
	IGUIScrollBar* Sound;
	IGUIScrollBar* Efect;
	IVideoModeList *modeList = device->getVideoModeList();

	ISoundEngine* engine = createIrrKlangDevice();
	ISoundEngine* dvigatel = createIrrKlangDevice();
	ISoundEngine* tormoz = createIrrKlangDevice();

	if (sounds == 1) { engine->play2D("../audio/Junkie XL.mp3", true); }
	SIrrlichtCreationParameters deviceParam;
	deviceParam.WindowSize.Width=1366;
	deviceParam.WindowSize.Height=768;

	IGUIComboBox *VideoMode = gui->addComboBox(rect<s32>( 550, 359, 760, 376 ), 0, GUI_ID_VIDEO_MODE);
	gui->clear();
	IGUIScrollBar* Gamma;

	IAnimatedMesh* mesh=smgr->getMesh("../models/Track_14.3ds");
	IAnimatedMesh* garage=smgr->getMesh("../models/Garage.3ds");
	ISceneNode *node = 0, *room = 0;
	node = smgr->addOctreeSceneNode(mesh->getMesh(0), 0, IDFlag_IsPickable);
	room=smgr->addMeshSceneNode(garage);
	node->setRotation(vector3df(0,90,0));
	room->setRotation(vector3df(0,-90,0));
	node->setScale(core::vector3df(30,23,30));
	room->setScale(core::vector3df(11,11,11));
	node->setMaterialFlag(video::EMF_LIGHTING, false);
	room->setMaterialFlag(video::EMF_LIGHTING, false);
	room->setPosition(vector3df(10000,0,300));

	ITriangleSelector* selector = 0;
	selector = smgr->createOctreeTriangleSelector(mesh->getMesh(0), node, 128);
	node->setTriangleSelector(selector);
	ICameraSceneNode* camera;
	ISceneNodeAnimator* anim;
	ISceneNode* highlightedSceneNode = 0;
	SMaterial material;
	ISceneNode* skydome;
	ISceneCollisionManager* collMan = smgr->getSceneCollisionManager();

	SAppContext context_game;
	context_game.device = device;
	context_game.gui=gui;
	context_game.menu=menu;
	context_game.VideoMode=VideoMode;
	context_game.car = car;
	MyEventReceiver receiver(context_game);
	device->setEventReceiver(&receiver);

	ITimer *time_game;
	ITimer *Old_time_game;
	context_game.menu=4;
	//////////////////////////////////////////////////////////////////////
	while(context_game.menu!=100)
	{
		if(context_game.menu==12)
		{
			engine->stopAllSounds();
			device=createDevice(EDT_DIRECT3D9, dimension2d<u32>(deviceParam.WindowSize.Width, deviceParam.WindowSize.Height), 32, true, false, false, 0);
			smgr=device->getSceneManager();
			gui=device->getGUIEnvironment();
			driver=device->getVideoDriver();
			if (sounds == 1 ) { engine->play2D("../audio/Junkie XL.mp3", true); }
			modeList = device->getVideoModeList();
			context_game.menu=0;
			new_game = 1;
		}

		speadXCeloe=0, Old_roat=0, read=0, start=0, new_game=1, SetIdent=0;
		roat=90, i=0, speed=0, spead_old=0, MoveX=-1, MoveZ=0, RXA=621, RZA=0, lookat_cameraOld=0;

		position_camera=vector3df(10721,220,300);
		lookat_camera=vector3df(1000,300,300);

		context_game.device = device;
		context_game.gui=gui;
		context_game.menu=menu;
		context_game.VideoMode=VideoMode;
		MyEventReceiver receiver(context_game);
		device->setEventReceiver(&receiver);

		Hero.Show();
		Hero.node->setID(IDFlag_IsPickable);

		camera = smgr->addCameraSceneNode(0,position_camera,lookat_camera, ID_IsNotPickable);
		anim = smgr->createCollisionResponseAnimator(selector, Hero.node, vector3df(30,60,30), vector3df(0,-10,0), vector3df(0,0,0));
		selector->drop();
		camera->addAnimator(anim);
		anim->drop();

		room->setVisible(true);
		node->setVisible(false);

		skydome=smgr->addSkyDomeSceneNode(driver->getTexture("../models/skydome.jpg"),16,8,0.95f,2.0f);
		driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, true);

		gui = device->getGUIEnvironment();
		IGUISkin* skin = gui->getSkin();
		IGUIFont* font = gui->getFont("../models/fonthaettenschweiler.bmp");
		if (font) skin->setFont(font);

		while(device->run())
		{
			if(device->isWindowActive())
			{
				camera = smgr->addCameraSceneNode(0,position_camera, lookat_camera, ID_IsNotPickable);
				camera->setFarValue(27000);
				ray.start = camera->getPosition();
				ray.end = ray.start + (camera->getTarget() - ray.start).normalize() * 700.0f;
				selectedSceneNode = collMan->getSceneNodeAndCollisionPointFromRay( ray, intersection, hitTriangle, IDFlag_IsPickable, 0); 

				if(selectedSceneNode)
				{
					speed=0;
					
					driver->setTransform(video::ETS_WORLD, core::matrix4());
					driver->setMaterial(material);
					driver->draw3DTriangle(hitTriangle, video::SColor(0,255,0,0));

					if((selectedSceneNode->getID() & IDFlag_IsHighlightable) == IDFlag_IsHighlightable)
					{
						highlightedSceneNode = selectedSceneNode;

						highlightedSceneNode->setMaterialFlag(video::EMF_LIGHTING, false);
					}
				}
			OptionsCreate=0;

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
				if (return_game == 1)
				{
					gui->clear();
					new_game = 1;
					engine->stopAllSounds();
					dvigatel->stopAllSounds();
					tormoz->stopAllSounds();
					device->getCursorControl()->setVisible(true);
					bot.Hide();
					if (sounds == 1) { engine->play2D("../audio/Junkie XL.mp3", true); }
					room->setVisible(true);
					node->setVisible(false);
					Hero.Hide();
					Hero.Create(textura, scale_start);
					Hero.hero_position = vector3df(9950, 0, 300);
					Hero.hero_roat = vector3df(0, -135, 0);
					Hero.node->setRotation(vector3df(0, -135, 0));
					Hero.Show();
					position_camera = vector3df(10721, 220, 300);
					lookat_camera = vector3df(1000, 300, 300);
					return_game = 0;
				}
				driver->beginScene(true, true, SColor(255,255,255,255));
				gui->addButton(rect< s32 >(10,252,200,296), 0, GUI_ID_CHOISE, L"Start");
				gui->addButton(rect< s32 >(10,306,200,348), 0, GUI_ID_GARAGE, L"Garage");
				gui->addButton(rect< s32 >(0,726,170,768), 0, GUI_ID_BACK, L"Back");
				smgr->drawAll();
				gui->drawAll();
				driver->endScene();
			}

			// Гараж
			if(context_game.menu==5)
			{
				driver->beginScene(true, true, SColor(255,255,255,255));
				if (carChoise != car_old)
				{
					gui->clear();
					gui->addButton(rect< s32 >(0, 726, 170, 768), 0, GUI_ID_BACK_KARER, L"Back");
					gui->addButton(rect< s32 >(600, 691, 670, 733), 0, GUI_ID_LAST_CAR, L"Last");
					gui->addButton(rect< s32 >(730, 691, 800, 733), 0, GUI_ID_NEXT_CAR, L"Next");
					stringw Money(money), Cena(cena);
					gui->addStaticText(L"	You Money", rect<s32>(530, 40, 600, 70), false, true, (IGUIElement*)0, -1, true);
					gui->addStaticText(Money.c_str(), rect<s32>(610, 40, 670, 70), false, true, (IGUIElement*)0, -1, true);
					gui->addStaticText(L"	Cena", rect<s32>(730, 40, 800, 70), false, true, (IGUIElement*)0, -1, true);
					gui->addStaticText(Cena.c_str(), rect<s32>(810, 40, 870, 70), false, true, (IGUIElement*)0, -1, true);
					car_old = carChoise;
				}
				if (context_game.car == 1)
				{
					Hero.Hide();
					textura = 1, carChoise = 1, cena = 40000;
					scale_start = vector3df(3.2, 3.2, 3.2);
					position_start = vector3df(9950, 0, 300);
					roat_start = vector3df(0, -135, 0);
					Hero.Create(textura, scale_start);
					Hero.hero_position = position_start;
					Hero.hero_roat = roat_start;
					Hero.Show();
				}
				if (context_game.car == 2)
				{
					Hero.Hide();
					textura = 8, carChoise = 2, cena = 37000;
					scale_start = vector3df(150, 150, 150);
					position_start = vector3df(9950, 0, 300);
					roat_start = vector3df(0, -135, 0);
					Hero.Create(textura, scale_start);
					Hero.hero_position = position_start;
					Hero.hero_roat = roat_start;
					Hero.Show();
				}
				if (context_game.car == 3)
				{
					Hero.Hide();
					textura = 3, carChoise = 3, cena = 120000;
					scale_start = vector3df(150, 150, 150);
					position_start = vector3df(9950, 0, 300);
					roat_start = vector3df(0, -135, 0);
					Hero.Create(textura, scale_start);
					Hero.hero_position = position_start;
					Hero.hero_roat = roat_start;
					Hero.Show();
				}
				if (context_game.car == 4)
				{
					Hero.Hide();
					textura = 9, carChoise = 4, cena = 150000;
					scale_start = vector3df(150, 150, 150);
					position_start = vector3df(9950, 0, 300);
					roat_start = vector3df(0, -135, 0);
					Hero.Create(textura, scale_start);
					Hero.hero_position = position_start;
					Hero.hero_roat = roat_start;
					Hero.Show();
				}
				smgr->drawAll();
				gui->drawAll();
				driver->endScene();
			}
			if (context_game.menu != 5 && car_old==carChoise) { car_old=0; }

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
				if(OptionsCreate==0)
				{
					gui->addStaticText(L"   Sound Volume",rect<s32>(450,200,535,235), false , true, (IGUIElement*)0, -1, true);
					Sound = gui->addScrollBar(true,rect<s32>(550,209,760,226),0,-1);
					gui->addStaticText(L"   Efect Volume",rect<s32>(450,250,535,285), false , true, (IGUIElement*)0, -1, true);
					Efect = gui->addScrollBar(true,rect<s32>(550,259,760,276),0,-1);
					gui->addButton(rect< s32 >(0,726,170,768), 0, GUI_ID_BACK, L"Back");
					gui->addStaticText(L"   Video Mode",rect<s32>(450,350,535,385), false , true, (IGUIElement*)0, -1, true);
					VideoMode = gui->addComboBox(rect<s32>( 550, 359, 760, 376 ), 0, GUI_ID_VIDEO_MODE);
					gui->addButton(rect< s32 >(780,357,820,375), 0, GUI_ID_SET, L"Set");
					gui->addStaticText ( L"   Gamma:", rect<s32>(450,300,535,335), false, true, 0, -1, true );
					Gamma = gui->addScrollBar( true, rect<s32>( 550, 310, 760, 327 ), 0, GUI_ID_TRANSPARENCY_SCROLL_BAR );
					Gamma->setMin ( 50 );
					Gamma->setMax(255);
					Gamma->setSmallStep ( 1 );
					Gamma->setLargeStep ( 2 );
					Gamma->setPos(gui->getSkin()->getColor(EGDC_WINDOW).getAlpha());
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
							VideoMode->addItem ( stringw ( buf ).c_str(), val );
							VideoMode->setSelected ( VideoMode->getIndexForItemData ( deviceParam.WindowSize.Width << 16 | deviceParam.WindowSize.Height ) );
							OptionsCreate=1;
						}
					}
				}

				smgr->drawAll();
				gui->drawAll();
				driver->endScene();
			}

			// Выбор Разрешения
			if(context_game.comboBox==1)
			{
				deviceParam.WindowSize.Width=receiver.width;
				deviceParam.WindowSize.Height=receiver.height;
				context_game.comboBox==0;
			}
			// Установка разрешение
			if (context_game.menu == 12)
			{
				device->closeDevice();
			}
			//Выбор трассы
			if (context_game.menu == 6)
			{
				driver->beginScene(true, true, SColor(255, 255, 255, 255));
				if (choise_track == 0)
				{
					gui->addButton(rect< s32 >(490, 252, 680, 452), 0, GUI_ID_TRACK1, L"Track 1");
					gui->addButton(rect< s32 >(700, 252, 890, 452), 0, GUI_ID_TRACK2, L"Track 2");
					gui->addButton(rect< s32 >(0, 726, 170, 768), 0, GUI_ID_BACK_KARER, L"Back");
					choise_track = 1;
				}
				smgr->drawAll();
				gui->drawAll();
				driver->endScene();
			}
			if (context_game.menu != 6 && choise_track==1) { choise_track = 0; }

			// Игра
			if(context_game.menu==2)
			{
				fps = driver->getFPS();
				if (lastFPS != fps)
				{
					stringw str = L"[";
					str += driver->getName();
					str += "] FPS:";
					str += fps;
					device->setWindowCaption(str.c_str());
					lastFPS = fps;
				}
				if(new_game==1)
				{
					engine->stopAllSounds();
					sound = rand() % 7;
					if (sounds == 1)
					{
						if (sound == 1) { engine->play2D("../audio/NFS.mp3"); }
						if (sound == 2) { engine->play2D("../audio/Junkie XL.mp3"); }
						if (sound == 3) { engine->play2D("../audio/Skindred.mp3"); }
						if (sound == 4) { engine->play2D("../audio/Static_X.mp3"); }
						if (sound == 5) { engine->play2D("../audio/BrokenPromis.mp3"); }
						if (sound == 6) { engine->play2D("../audio/Overseer.mp3"); }
					}
					if (context_game.track == 1)
					{
						Hero.hero_position = vector3df(position_start.X-50, position_start.Y, position_start.Z - 215);
						Hero.hero_roat = vector3df(roat_start.X, roat_start.Y - 135, roat_start.Z);
						Hero.node->setScale(scale_start);
						position_camera = vector3df(10521, 250, 85);
						lookat_camera = vector3df(1000, 200, 85);
						Hero.Show();
					}
					if (context_game.track == 2)
					{
						Hero.hero_position = vector3df(-56000, position_start.Y, 28000);
						Hero.hero_roat = vector3df(roat_start.X, -90, roat_start.Z);
						Hero.node->setScale(scale_start);
						position_camera = vector3df(-56571, 250, 28000);
						lookat_camera = vector3df(40000, 200, 28000);
					}
					room->setVisible(false);
					node->setVisible(true);
					//if (bot_delete==1) { bot.Hide(); }
					time_game = device->getTimer();
					bot_delete = 1, Old_roat = 0, roat = 90, speed = 0, read = 0, new_game = 0, return_game = 1, Old_time_game = 0, choise_track = 0, 
						MoveX = -1, MoveZ = 0, RXA = 621, RZA = 0, bot_MoveX = 0, bot_MoveZ = 0, bot_roat = 90, num = 0;
					bot.Create(1, vector3df(3.2, 3.2, 3.2));
					bot.bot_roat = vector3df(0, 90, 0);
					bot.bot_position = vector3df(10000, 50, 500);
					bot.Show();
					FILE *Move = fopen("../bot_move.txt", "rt");
					Bot_posX[0] = bot.bot_position.X;
					Bot_posZ[0] = bot.bot_position.Z;
					Bot_roat[0] = bot.bot_roat.Y;
					for (int i = 1; i < 32; i++)
					{
						fscanf(Move, "%d%d%d", &Bot_posX[i], &Bot_posZ[i], &Bot_roat[i]);
					}
					fclose(Move);
					device->getCursorControl()->setVisible(false);
				}
				// Bot
				if (bot.bot_position.X == Bot_posX[num] && num<31)
				{
					bot_MoveX = (Bot_posX[num+1] - Bot_posX[num]);
					bot_MoveX /= 200;
					bot_MoveZ = (Bot_posZ[num+1] - Bot_posZ[num]);
					bot_MoveZ /= 200;
					num++;
				}
				if (bot_roat < Bot_roat[num])
				{
					bot_roat += 0.5;
					bot.node->setRotation(vector3df(0, bot_roat, 0));
					bot.bot_roat.Y = bot_roat;
				}
				if (bot_roat > Bot_roat[num])
				{
					bot_roat -= 0.5;
					bot.node->setRotation(vector3df(0, bot_roat, 0));
					bot.bot_roat.Y = bot_roat;
				}
				bot.Move(bot_MoveX, 0, bot_MoveZ);

				if (Hero.hero_position.X <= -54000 && Hero.hero_position.Z>25500 && Hero.hero_position.Z<28500)
				{
					gui->addStaticText(L"									Finish", rect<s32>(633, 350, 733, 400), false, true, (IGUIElement*)0, -1, true);
					win = 1;
					if (speed>0)
					{
						speed -= 3;
					}
					else
						speed = 0;
						money += 100;
				}

				if (win = 1) {}

				if(bot.node!=NULL)
				{
					bboxHero =  Hero.node->getTransformedBoundingBox();
					bboxHero.MinEdge+=Hero.node->getPosition();
					bboxHero.MaxEdge+=Hero.node->getPosition();
					bboxStreetCar =  bot.node->getTransformedBoundingBox();
					bboxStreetCar.MinEdge+=bot.node->getPosition();
					bboxStreetCar.MaxEdge+=bot.node->getPosition();

					if(bboxHero.intersectsWithBox(bboxStreetCar)) { speed=0; }
				}

				Hero.Move(speed*MoveX, 0, speed*MoveZ, textura, scale_start);
				lookat_camera.X+=MoveX*speed;
				lookat_camera.Z+=MoveZ*speed;
				camera->setPosition(vector3df(position_camera.X+=speed*MoveX,position_camera.Y,position_camera.Z+=speed*MoveZ));

				if(speed<=0)
				{
					dvigatel->stopAllSounds(); 
					gui->addStaticText(L"0",rect<s32>(107,630,128,660), false , true, (IGUIElement*)0, -1, true);
					gui->addStaticText(L"M/H",rect<s32>(128,630,148,660), false , true, (IGUIElement*)0, -1, true);
				}

				if(speed>0)
				{
					speadXCeloe=speed;
					gui->clear();
					stringw Speed(speadXCeloe);
					stringw time(timeCeloe);
					gui->addStaticText(Speed.c_str(),rect<s32>(107,630,128,660), false , true, (IGUIElement*)0, -1, true);
					gui->addStaticText(L"M/H",rect<s32>(128,630,148,660), false , true, (IGUIElement*)0, -1, true);
					//env->addStaticText(time.c_str(),rect<s32>(670,50,730,100), false , true, (IGUIElement*)0, -1, true);

				}
				if(speed<0)
				{
					speed+=0.02;
				}
				//Кнопки
				if (receiver.IsKeyDown(irr::KEY_KEY_G))
				{
					cout << "pos X = " << Hero.hero_position.X << endl;
					cout << "pos Z = " << Hero.hero_position.Z << endl;
					cout << "roat = "  << roat << endl;
				}
				if (receiver.IsKeyDown(irr::KEY_ESCAPE))
				{
					context_game.menu = 1;
				}
				if(receiver.IsKeyDown(irr::KEY_KEY_W))
				{
					if(speed<15)
					{ 
						speed+=0.17; 
						if(speed<=0.18)
						{
							dvigatel->play2D("../audio/dvigatel.mp3",true);
							dvigatel->setSoundVolume(0.2);
						}
					}
					if(speed>=15 && speed<50) { speed+=0.18; }
					if(speed>=50 && speed<140) {	speed+=0.2; }
					else {speed+=0.001; }
				}

				if(receiver.IsKeyDown(irr::KEY_KEY_S))
				{
					speed-=0.1;
					tormoz->stopAllSounds();
					tormoz->play2D("../audio/Tormozov.mp3");
					if(speed>0) {speed-=0.4;}
				}

				if(receiver.IsKeyDown(irr::KEY_KEY_A))
				{
					roat-=1;
					Hero.node->setRotation(vector3df(roat_start.X,roat,roat_start.Z)); // поворот модели на месте
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
					Hero.node->setRotation(vector3df(roat_start.X, roat, roat_start.Z)); // поворот модели на месте
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

#include<irrlicht.h>
#include<iostream>
#pragma once
using namespace irr;
using namespace core;
using namespace video;
using namespace gui;
using namespace std;

struct SAppContext
{
	IrrlichtDevice *device;
	IGUIEnvironment *gui;
	s32 menu;
	s32 comboBox;
	s32 old_menu;
	s32 Width;
	s32 Height;
	IGUIComboBox *VideoMode;
};

enum
{
	GUI_ID_KARER,
	GUI_ID_FAST_START,
	GUI_ID_STATISTIC,
	GUI_ID_OPTIONS,
	GUI_ID_START,
	GUI_ID_GARAGE,
	GUI_ID_BACK,
	GUI_ID_BACK_KARER,
	GUI_ID_BACK_GARAGE,
	GUI_ID_TUNING,
	GUI_ID_STAILING,
	GUI_ID_CAR,
	GUI_ID_SET,
	GUI_ID_NEXT_CAR,
	GUI_ID_LAST_CAR,
	GUI_ID_MEIN_MENU,
	GUI_ID_TRANSPARENCY_SCROLL_BAR,
	GUI_ID_VIDEO_MODE,
	GUI_ID_QUIT,
};

class MyEventReceiver : public IEventReceiver
{
public:
	SAppContext& Context;
	bool KeyIsDown[KEY_KEY_CODES_COUNT];
	virtual bool OnEvent(const SEvent& event);
	virtual bool IsKeyDown(EKEY_CODE keyCode);
	MyEventReceiver(SAppContext& Context);
	~MyEventReceiver();
};


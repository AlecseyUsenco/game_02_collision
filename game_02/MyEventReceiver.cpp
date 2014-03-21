#include "MyEventReceiver.h"


MyEventReceiver::MyEventReceiver(SAppContext& Context) : Context(Context)
{
	for (u32 i=0; i<100; i++)
	{
		KeyIsDown[i] = false;
	}
}

bool MyEventReceiver::OnEvent(const SEvent& event)
{
	if(event.EventType == irr::EET_KEY_INPUT_EVENT)
	{
		KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;
		return false;
	}
	if(event.EventType == EET_GUI_EVENT)
	{
		s32 id = event.GUIEvent.Caller->getID();
		IGUIEnvironment* env = Context.device->getGUIEnvironment();
		switch(event.GUIEvent.EventType)
		{
			case EGET_BUTTON_CLICKED:
			switch(id)
			{
				case GUI_ID_KARER:
				Context.gui->clear();
				Context.menu=1;
				Context.old_menu=0;
				return true;
				break;

				case GUI_ID_FAST_START:
				Context.gui->clear();
				Context.menu=2;
				Context.old_menu=0;
				return true;
				break;

				case GUI_ID_STATISTIC:
				Context.gui->clear();
				Context.menu=3;
				Context.old_menu=0;
				return true;
				break;

				case GUI_ID_OPTIONS:
				Context.gui->clear();
				Context.menu=4;
				Context.old_menu=0;
				return true;
				break;

				case GUI_ID_START:
				Context.gui->clear();
				Context.menu=2;
				return true;
				break;

				case GUI_ID_GARAGE:
				Context.gui->clear();
				Context.menu=5;
				return true;
				break;

				case GUI_ID_TUNING:
				Context.gui->clear();
				Context.menu=6;
				return true;
				break;

				case GUI_ID_STAILING:
				Context.gui->clear();
				Context.menu=7;
				return true;
				break;

				case GUI_ID_CAR:
				Context.gui->clear();
				Context.menu=8;
				return true;
				break;

				case GUI_ID_QUIT:
				Context.device->closeDevice();
				Context.menu=100;
				return true;
				break;

				case GUI_ID_BACK:
				Context.gui->clear();
				Context.menu=Context.old_menu;
				return true;
				break;

				case GUI_ID_BACK_KARER:
				Context.gui->clear();
				Context.menu=1;
				return true;
				break;

				case GUI_ID_BACK_GARAGE:
				Context.gui->clear();
				Context.menu=5;
				return true;
				break;

				case GUI_ID_SET:
				Context.gui->clear();
				Context.menu=12;
				return true;
				break;

				default:
				return false;
			}
			break;

			case EGET_SCROLL_BAR_CHANGED:
			if (id == GUI_ID_TRANSPARENCY_SCROLL_BAR)
			{
				s32 pos = ((IGUIScrollBar*)event.GUIEvent.Caller)->getPos();
				for (u32 i=0; i<500; i++)
				{
					SColor col = env->getSkin()->getColor((EGUI_DEFAULT_COLOR)i);
					col.setAlpha(pos);
					env->getSkin()->setColor((EGUI_DEFAULT_COLOR)i, col);
				}
			}
			return true;
			break;

			case EGET_COMBO_BOX_CHANGED:
			if(id == GUI_ID_VIDEO_MODE)
			{
				u32 val = Context.VideoMode->getItemData ( Context.VideoMode->getSelected() );
				Context.Width = val >> 16;
				Context.Height = val & 0xFFFF;
				//Context.VideoMode->setSelected (  val );
				cout<<Context.Width<<endl;
				cout<<Context.Height<<endl;
			}
			return true;
			break;

			default:
			break;
		}
	}
	return false;
}

bool MyEventReceiver::IsKeyDown(EKEY_CODE keyCode)
{
	return KeyIsDown[keyCode];
}

MyEventReceiver::~MyEventReceiver()
{
}

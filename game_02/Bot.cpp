#include "Bot.h"


Bot::Bot()
{
	bot_roat = vector3df(0, 0, 0);
	bot_position = vector3df(10000, 50, 500);
}

Bot::~Bot()
{
}

void Bot::BotPosition(float dx, float dy, float dz)
{
	bot_position = vector3df(this->bot_position.X + dx, this->bot_position.Y + dy, this->bot_position.Z + dz);
}

void Bot::Show()
{
	node->setRotation(bot_roat);
	node->setPosition(bot_position);
	node->setVisible(true);
}

void Bot::Move(float dx, float dy, float dz)
{
	Hide();
	Create(0, vector3df(1.45, 1.45, 1.45));
	BotPosition(dx, dy, dz);
	Show();
}
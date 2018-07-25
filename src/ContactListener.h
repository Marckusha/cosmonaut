#pragma once
#include "Box2D/Box2D.h"
#include "Player.h"

class ContactListener : public b2ContactListener {
public:
	ContactListener();
	ContactListener(std::shared_ptr<Player> player, GUI::Widget* widget);

	void BeginContact(b2Contact* contact);
	void EndContact(b2Contact* contact);

private:
	std::shared_ptr<Player> _player;
	GUI::Widget* _widget;
};

#include "stdafx.h"
#include "ContactListener.h"

ContactListener::ContactListener()
	: b2ContactListener()
	, _player()
{}

ContactListener::ContactListener(std::shared_ptr<Player> player, GUI::Widget* widget)
	: b2ContactListener()
{
	_player = player;
	_widget = widget;
}

void ContactListener::BeginContact(b2Contact* contact) {
	auto first = contact->GetFixtureA()->GetBody()->GetUserData();
	auto second = contact->GetFixtureB()->GetBody()->GetUserData();
	if ((second == "player" && first == "wall") ||
		(first == "player" && first == "wall")) {
		Message message("PlayerCollision");
		_widget->AcceptMessage(message);
	}
	//TODO
	//пересмотреть архитектуру регистрации столкновений
	if (second == "player" && (first == "Powerup" || first == "RepairKit")) {
		contact->GetFixtureA()->GetBody()->SetUserData("delete");
		const char* str = static_cast<const char*>(first);
		Message message(str);
		_widget->AcceptMessage(message);
	}
	else if (first == "player" && (second == "Powerup" || second == "RepairKit")) {
		contact->GetFixtureB()->GetBody()->SetUserData("delete");
		const char* str = static_cast<const char*>(second);
		Message message(str);
		_widget->AcceptMessage(message);
	}
}

void ContactListener::EndContact(b2Contact* contact) {

}
#pragma once
#include "Node.h"
#include "Debris.h"
#include "Player.h"
#include "Bonus.h"

class DynamicObjectFactory {
public:
	DynamicObjectFactory();

	std::shared_ptr<Debris> CreateDebris(const FPoint& pos, const std::string& nameText,
		float density, float restitution, Node::TypeShape type);

	std::shared_ptr<Debris> CreateDebris(const FPoint& pos, const std::string& nameText,
		float density, float restitution, const std::vector<b2Vec2>& vert);

	std::shared_ptr<Bonus> CreateBonus(const FPoint& pos, const std::string& nameText,
		float density, float restitution, Node::TypeShape type);

	std::shared_ptr<Player> CreatePlayer(const FPoint& pos, const std::string& nameText,
		float density, float restitution, Node::TypeShape type);
};

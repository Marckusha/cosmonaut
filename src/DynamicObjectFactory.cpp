#include "stdafx.h"
#include "DynamicObjectFactory.h"

DynamicObjectFactory::DynamicObjectFactory() {}

std::shared_ptr<Bonus> DynamicObjectFactory::CreateBonus(const FPoint& pos, const std::string& nameText,
	float density, float restitution, Node::TypeShape type) 
{
	std::shared_ptr<Bonus> node = std::shared_ptr<Bonus>(new Bonus(pos, nameText, density, restitution, type));
	return node;
}

std::shared_ptr<Debris> DynamicObjectFactory::CreateDebris(const FPoint& pos, const std::string& nameText,
	float density, float restitution, Node::TypeShape type) 
{
	std::shared_ptr<Debris> node = std::shared_ptr<Debris>(new Debris(pos, nameText, density, restitution, type));
	return node;
}

std::shared_ptr<Debris> DynamicObjectFactory::CreateDebris(const FPoint& pos, const std::string& nameText,
	float density, float restitution, const std::vector<b2Vec2>& vert)
{
	std::shared_ptr<Debris> node = std::shared_ptr<Debris>(new Debris(pos, nameText, density, restitution, vert));
	return node;
}

std::shared_ptr<Player> DynamicObjectFactory::CreatePlayer(const FPoint& pos, const std::string& nameText,
	float density, float restitution, Node::TypeShape type) 
{
	std::shared_ptr<Player> node = std::shared_ptr<Player>(new Player(pos, nameText, density, restitution, type));
	return node;
}
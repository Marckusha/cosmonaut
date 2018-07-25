#include "stdafx.h"
#include "Debris.h"

Debris::Debris() 
	: Node()
{	}

Debris::Debris(const FPoint& pos, const std::string& nameText,
	float density, float restitution, Node::TypeShape type)
	: Node(pos, nameText, density, restitution, type)
{	
	_gravityComponent = GravityComponent(_body);
}

Debris::Debris(const FPoint& pos, const std::string& nameText,
	float density, float restitution, const std::vector<b2Vec2>& vertex)
	: Node(pos, nameText, density, restitution, vertex)
{
	_gravityComponent = GravityComponent(_body);
}

void Debris::Draw() {
	Node::Draw();
}

void Debris::Update(float dt) {
	_gravityComponent.Update(dt);
}

void Debris::MouseDownLeft(const IPoint& mouse_pos) {
	_gravityComponent.ApplyForceRepellent(mouse_pos);
}

void Debris::MouseDownRight(const IPoint& mouse_pos) {
	_gravityComponent.ApplyForceGravity(mouse_pos);
}

void Debris::MouseUp(const IPoint& mouse_pos) {
	_gravityComponent.StopForce();
}

Debris::~Debris() {}
#include "stdafx.h"
#include "Bonus.h"

Bonus::Bonus() 
	: Node()
{	}

Bonus::Bonus(const FPoint& pos, const std::string& nameText,
	float density, float restitution, TypeShape type) 
	: Node(pos, nameText, density, restitution, type)
{	}

void Bonus::Draw() {
	Node::Draw();
}

void Bonus::Update(float dt) {}

Bonus::~Bonus() {}
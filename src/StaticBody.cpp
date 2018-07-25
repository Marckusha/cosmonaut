#include "stdafx.h"
#include "StaticBody.h"
#include "Variables.h"
#include "Config.h"

using variable::SCALE;

StaticBody::StaticBody()
{}

StaticBody::StaticBody(const FPoint& pos, const FPoint& size) 
	: _position(pos)
	, _size(size)
{
	Config& instance = Config::GetInstance();
	auto world = instance.GetWorld();

	b2BodyDef bodyDef;
	bodyDef.position.Set(pos.x / SCALE, pos.y / SCALE);
	_body = world->CreateBody(&bodyDef);

	b2PolygonShape shape;
	shape.SetAsBox(size.x / SCALE, size.y / SCALE);

	b2FixtureDef fixDef;
	fixDef.shape = &shape;
	_body->CreateFixture(&fixDef);
}
#pragma once
#include "Box2D/Box2D.h"

class StaticBody {
public:
	StaticBody();
	StaticBody(const FPoint& pos, const FPoint& size);

	inline void StaticBody::SetUserData(void* data) { _body->SetUserData(data); }

private:
	FPoint _position = FPoint();
	FPoint _size = FPoint();
	b2Body * _body;
};
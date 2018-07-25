#pragma once
#include "Box2D/Box2D.h"

class GravityComponent {
public:
	GravityComponent();
	GravityComponent(b2Body* body, float gravCoef = 1.f);

	inline void SetGravityCoeff(float coeff) { _gravityCoeff = coeff; }

	void ApplyForceGravity(const FPoint& vec);
	void ApplyForceRepellent(const FPoint& vec);
	void StopForce();

	void Update(float dt);

private:
	void ApplyForce(const FPoint& vec, float force);

private:
	float _gravityFadeInTime = 3.f; //время нарастания мощности
	float _gravityCoeff = 1.f;
	bool _isGravity = false;		//наличие гравитации
	bool _isRepellent = false;		//наличие отталкивания
	FPoint _mousePos = FPoint();
	b2Body* _body = nullptr;
};
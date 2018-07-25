#include "stdafx.h"
#include "Variables.h"
#include "GravityComponent.h"
#include "Config.h"

using variable::SCALE;
using variable::PI;

//возвращает силу, с которой действует гравитация на тело
float GetForce(const FPoint& mousePos, const FPoint& bodyPos);
//возвращает угол вектора от тела до точки гравитации
float GetRotate(const FPoint& mousePos, const FPoint& bodyPos);
float GetForceForRepellent(const FPoint& mousePos, const FPoint& bodyPos);

GravityComponent::GravityComponent()
{	}

GravityComponent::GravityComponent(b2Body* body, float gravCoef)
	: _body(body)
	, _gravityCoeff(gravCoef)
{	}

void GravityComponent::Update(float dt) {

	Config& instance = Config::GetInstance();	
	float GravityFadeInTime  = instance.GetConstantsByKey("GravityFadeInTime");

	if (_isGravity) {
		ApplyForceGravity(_mousePos);
		if (_gravityFadeInTime < GravityFadeInTime) {
			_gravityFadeInTime += dt;
		}
	}
	else if (_isRepellent) {
		ApplyForceRepellent(_mousePos);
		if (_gravityFadeInTime < GravityFadeInTime) {
			_gravityFadeInTime += dt;
		}
	}
}

void GravityComponent::ApplyForceGravity(const FPoint& vec) {

	Config& instance = Config::GetInstance();
	float GravityFadeInTime = instance.GetConstantsByKey("GravityFadeInTime");

	_isGravity = true;
	FPoint bodyPos = FPoint(_body->GetPosition().x * SCALE, _body->GetPosition().y * SCALE);
	float force = GetForce(vec, bodyPos) * _gravityCoeff;

	ApplyForce(vec, force * _gravityFadeInTime / GravityFadeInTime);
}

void GravityComponent::ApplyForceRepellent(const FPoint& vec) {
	Config& instance = Config::GetInstance();
	float GravityFadeInTime = instance.GetConstantsByKey("GravityFadeInTime");

	_isRepellent = true;
	FPoint bodyPos = FPoint(_body->GetPosition().x * SCALE, _body->GetPosition().y * SCALE);
	float force = GetForce(vec, bodyPos) * _gravityCoeff;

	ApplyForce(vec, -force * _gravityFadeInTime / GravityFadeInTime);
}

void GravityComponent::ApplyForce(const FPoint& vec, float force) {
	_mousePos = vec;

	FPoint bodyPos = FPoint(_body->GetPosition().x * SCALE, _body->GetPosition().y * SCALE);

	float rotate = GetRotate(vec, bodyPos);
	float antiPI = 4.f * atan(1.f) / 180.f;
	float dtX = -sin(rotate * antiPI);
	float dtY = cos(rotate * antiPI);

	_body->ApplyForceToCenter(b2Vec2(dtX * force, dtY * force), true);
}

void GravityComponent::StopForce() {
	_isGravity = false;
	_isRepellent = false;
	_gravityFadeInTime = 0.f;
	_body->SetLinearDamping(0.f);
}

float GetForce(const FPoint& mousePos, const FPoint& bodyPos) {
	float force = 0.f;

	float x = mousePos.x - bodyPos.x;
	float y = mousePos.y - bodyPos.y;
	float dt = sqrt(x * x + y * y);
	
	//максимальный радиус действия гравитации
	float MaxGravityDistance = Render::device.Width() / 4.f;
	float MinGravityDistance = 0.5f;
	float GravityCutoffDistance = 45.f;

	if (dt <= MaxGravityDistance && dt >= MinGravityDistance) {
		float speed = 800.f;
		float k = MaxGravityDistance / speed;
		force = speed - dt / k;
	}
	else {
		force = 0.f;
	}
	return force;
}

float GetForceForRepellent(const FPoint& mousePos, const FPoint& bodyPos) {
	float force = 0.f;

	float x = mousePos.x - bodyPos.x;
	float y = mousePos.y - bodyPos.y;
	float dt = sqrt(x * x + y * y);

	//максимальный радиус действия гравитации
	float MaxGravityDistance = Render::device.Width() / 2.f;
	float MinGravityDistance = 9.f;
	float GravityCutoffDistance = 45.f;

	if (dt <= MaxGravityDistance && dt >= MinGravityDistance) {
		float speed = 400.f;
		float k = MaxGravityDistance / speed;
		force = speed - dt / k;
	}
	else {
		force = 0.f;
	}
	return force;
}

float GetRotate(const FPoint& mousePos, const FPoint& bodyPos) {
	float rotate = 0.f;

	if (bodyPos.y < mousePos.y && bodyPos.x < mousePos.x) {
		rotate = 360 - std::atan(float((mousePos.x - bodyPos.x) / (mousePos.y - bodyPos.y))) * PI;
	}
	else if (bodyPos.y < mousePos.y && bodyPos.x > mousePos.x) {
		rotate = std::atan(float((bodyPos.x - mousePos.x) / (mousePos.y - bodyPos.y))) * PI;
	}
	else if (bodyPos.x < mousePos.x && bodyPos.y > mousePos.y) {
		rotate = 180 + std::atan(float((mousePos.x - bodyPos.x) / (bodyPos.y - mousePos.y))) * PI;
	}
	else if (bodyPos.x > mousePos.x && bodyPos.y > mousePos.y) {
		rotate = 180 - std::atan(float((bodyPos.x - mousePos.x) / (bodyPos.y - mousePos.y))) * PI;
	}

	return rotate;
}
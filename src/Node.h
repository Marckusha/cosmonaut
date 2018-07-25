#pragma once
#include "Box2D/Box2D.h"
#include "Variables.h"

class Node {
public:
	enum TypeShape {
		Box,
		Circle
	};

	Node();
	Node(const FPoint& pos, const std::string& nameText, 
		float density, float restitution, TypeShape type);

	//если нужно построить произвольный многоугольник, 
	//то передаем в последний аргумент координаты его вершин
	Node(const FPoint& pos, const std::string& nameTex,
		float density, float restitution, const std::vector<b2Vec2>& vertex);

	inline void SetLinearDamping(float damping) { _body->SetLinearDamping(damping); }
	inline void SetUserData(void* data) { _body->SetUserData(data); }
	inline void SetVelocity(const b2Vec2& vel) { _body->SetLinearVelocity(vel); }

	inline FPoint GetPosition() const { 
		return FPoint(_body->GetPosition().x * variable::SCALE, _body->GetPosition().y * variable::SCALE); 
	}
	inline b2Body* GetBody() { return _body; }
	
	virtual void MouseDownLeft(const IPoint& mouse_pos);
	virtual void MouseDownRight(const IPoint& mouse_pos);
	virtual void MouseUp(const IPoint& mouse_pos);


	virtual void Draw();
	virtual void Update(float dt);
	virtual ~Node();

private:
	void InitShape(TypeShape type);

protected:
	b2Shape * _shape = nullptr;
	b2Body * _body = nullptr;
	Render::Texture* _texture = nullptr;

private:
	bool _isPolygon = false;
};



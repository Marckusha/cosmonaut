#pragma once
#include "Node.h"
#include "GravityComponent.h"

class Debris : public Node {
public:
	Debris();

	Debris(const FPoint& pos, const std::string& nameText,
		float density, float restitution, Node::TypeShape type);

	Debris(const FPoint& pos, const std::string& nameText,
		float density, float restitution, const std::vector<b2Vec2>& vertex);

	inline void SetGravityCoeff(float coeff) { _gravityComponent.SetGravityCoeff(coeff); }

	virtual void MouseDownLeft(const IPoint& mouse_pos) override;
	virtual void MouseDownRight(const IPoint& mouse_pos) override;
	virtual void MouseUp(const IPoint& mouse_pos) override;

	virtual void Draw() override;
	virtual void Update(float dt) override;
	virtual ~Debris();

private:
	GravityComponent _gravityComponent = GravityComponent();
};

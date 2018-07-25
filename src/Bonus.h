#pragma once
#include "Node.h"

class Bonus : public Node {
public:
	Bonus();

	Bonus(const FPoint& pos, const std::string& nameText,
		float density, float restitution, TypeShape type);

	void Draw() override;
	void Update(float dt) override;

	virtual ~Bonus();
};

#pragma once
#include "Node.h"
#include "GravityComponent.h"

class Player : public Node {
public:
	Player();
	Player(const FPoint& pos, const std::string& nameText,
		float density, float restitution, Node::TypeShape type);

	inline float GetOxygen() const { return _oxygen; }
	inline bool IsDeath() const { return _isRestart; }
	inline void SetGravityCoeff(float cf) { _gravityComponent.SetGravityCoeff(cf); }

	void Collision();
	void FillOxygen();
	void FillHealth();

	void MouseDownLeft(const IPoint& mouse_pos) override;
	void MouseDownRight(const IPoint& mouse_pos) override;
	void MouseUp(const IPoint& mouse_pos) override;

	void Draw() override;
	void Update(float dt) override;

	virtual ~Player();
private:
	void UpdateSuit();
	void Init();

private:
	GravityComponent _gravityComponent = GravityComponent();
	Render::Texture * _text = nullptr;

	FlashAnimationPtr _flashAnimation = nullptr;

	bool _isDeath = false;			//регистрирует смерть игрока
	bool _isRestart = false;
	bool _isAnimation = false;		//проигрывание анимации
									//1 - проигрывать	

	float _timeAnimation = 0.f;		//время проигрывания анимации
	float _health = 100.f;			//hp игрока
	float _oxygen = 100.f;
	float _maxSpeed = 0.f;
};

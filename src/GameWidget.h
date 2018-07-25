#pragma once
#include "Node.h"
#include "Debris.h"
#include "Player.h"
#include "ContactListener.h"
#include "TMXTiledMap.h"
#include "StaticBody.h"
#include "MouseAnimate.h"

class GameWidget : public GUI::Widget
{
public:
	GameWidget(const std::string& name, rapidxml::xml_node<>* elem);

	void Draw() override;
	void Update(float dt) override;

	void AcceptMessage(const Message& message) override;

	bool MouseDown(const IPoint& mouse_pos) override;
	void MouseUp(const IPoint& mouse_pos) override;

private:
	void Init();
	void GenerateDebris();
	void InitDynamicObjects();
	void Restart();

private:
	bool _isDeleteBonus = false;
	int _idSample = 0;
	bool _isMouseClick = false;
	float _timerMusic = 0.f;

	Render::Texture* _backgroundTexture;
	Render::Texture* _exitTexture;
	Render::Texture* _interfaceTexture;
	Render::Texture* _barO2Texture;
	Render::Texture* _barO2bgTexture;

	std::shared_ptr<b2World> _world;

	std::vector< std::shared_ptr<Debris> > _debris;
	std::vector< std::shared_ptr<Node> > _dynamicObjects;
	std::shared_ptr<Player> _player;

	MouseAnimate _mouseAnimate = MouseAnimate();
	ContactListener _contactListener;
	TMXTiledMap _map = TMXTiledMap();
	FPoint _camera = FPoint();
};
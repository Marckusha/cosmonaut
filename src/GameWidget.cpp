#include "stdafx.h"
#include "GameWidget.h"
#include "StaticBody.h"
#include "Variables.h"
#include "DynamicObjectFactory.h"
#include "Bonus.h"
#include "Config.h"

std::vector<b2Vec2> GetVertexShape(const std::string& nameTex);
b2Vec2 RandomVelocity();

GameWidget::GameWidget(const std::string& name, rapidxml::xml_node<>* elem)
	: Widget(name)
{
	Init();
}

void GameWidget::Init() {
	Config& instance = Config::GetInstance();
	_world = instance.GetWorld();

	_backgroundTexture = Core::resourceManager.Get<Render::Texture>("Background");
	_exitTexture = Core::resourceManager.Get<Render::Texture>("Exit");
	_interfaceTexture = Core::resourceManager.Get<Render::Texture>("interface_O2");
	_barO2Texture = Core::resourceManager.Get<Render::Texture>("bar_O2");
	_barO2bgTexture = Core::resourceManager.Get<Render::Texture>("bar_O2_bg");
	
	_map.InitWithTMXFile("levels/Level1.xml");

	GenerateDebris();
	InitDynamicObjects();

	_contactListener = ContactListener(_player, this);
	_world->SetContactListener(&_contactListener);

	MM::manager.PlaySample("Ambient5");
}

void GameWidget::Restart() {
	_idSample = 0;
	_isMouseClick = false;
	_isDeleteBonus = false;
	_timerMusic = 0.f;
	MM::manager.StopAll();
	MM::manager.PlaySample("Ambient5");

	_camera = FPoint();

	_debris.clear();
	_dynamicObjects.clear();
	
	GenerateDebris();
	InitDynamicObjects();

	_contactListener = ContactListener(_player, this);
	_world->SetContactListener(&_contactListener);

	MM::manager.PlaySample("Ambient5");	
}

void GameWidget::GenerateDebris() {
	srand(time(0));
	Config& instance = Config::GetInstance();

	float DustPerMeter = instance.GetConstantsByKey("DustPerMeter");
	float GravityCoeffDust = instance.GetConstantsByKey("GravityCoeffDust");
	float DustDensity = instance.GetConstantsByKey("DustDensity");

	for (int i = _map.GetMapSize().x * _map.GetTileSize().x - DustPerMeter * 2; i > 0; i -= DustPerMeter *2) {
		for (int j = _map.GetMapSize().y * _map.GetTileSize().y - DustPerMeter * 2; j > 0; j -= DustPerMeter *2) {
			int max = rand() % 5;
			for (int k = 0; k < max; ++k) {

				b2Vec2 vel = RandomVelocity();

				int trashRand = rand() % 14 + 1;
				float widthTrash = rand() % int(DustPerMeter) + i;
				float heightTrash = rand() % int(DustPerMeter) + j;
				std::string trash = "Trash" + std::to_string(trashRand);

				std::shared_ptr<Debris> debris(new Debris(FPoint(widthTrash, heightTrash), 
					trash.c_str(), DustDensity, 0.1f, Node::TypeShape::Box));
				debris->SetGravityCoeff(GravityCoeffDust);
				debris->SetVelocity(vel);
				_debris.push_back(debris);
			}
		}
	}	
}

void GameWidget::InitDynamicObjects() {
	srand(time(0));
	auto mulmap = _map.GetMultiMapDynamicObjects();

	std::shared_ptr<DynamicObjectFactory> factory(new DynamicObjectFactory());

	Config& instance = Config::GetInstance();

	float GravityCoeffPlayer = instance.GetConstantsByKey("GravityCoeffPlayer");
	float GravityCoeffDebris = instance.GetConstantsByKey("GravityCoeffDebris");
	float PlayerDensity = instance.GetConstantsByKey("PlayerDensity");
	float DebrisDensity = instance.GetConstantsByKey("DebrisDensity");
	float PowerupDensity = instance.GetConstantsByKey("PowerupDensity");

	for (auto it = mulmap.begin(); it != mulmap.end(); ++it) {
		b2Vec2 vel = RandomVelocity();
		if (it->first == "Player") {

			std::shared_ptr<Player> player = 
				factory->CreatePlayer(it->second, it->first.c_str(), PlayerDensity, 0.f, Node::TypeShape::Circle); 

			player->SetUserData("player");
			player->SetGravityCoeff(GravityCoeffPlayer);
			player->SetLinearDamping(0.3f);
			_player = player;
		}
		else if (it->first == "Powerup" || it->first == "RepairKit") {

			std::shared_ptr<Bonus> bonus = 
				factory->CreateBonus(it->second, it->first.c_str(), PowerupDensity, 0.f, Node::TypeShape::Box);

			if (it->first == "Powerup") {
				bonus->SetUserData("Powerup");
			}
			else if (it->first == "RepairKit") {
				bonus->SetUserData("RepairKit");
			}
			bonus->SetVelocity(vel);
			_dynamicObjects.push_back(bonus);
		}
		else {
			std::shared_ptr<Debris> debris;
			auto vertex = GetVertexShape(it->first);
			if (vertex.empty()) {
				debris = factory->CreateDebris(it->second, it->first.c_str(), DebrisDensity, 0.1f, Node::TypeShape::Box);
			}
			else {
				debris = factory->CreateDebris(it->second, it->first.c_str(), DebrisDensity, 0.1f, vertex);
			}

			debris->SetUserData("wall");
			debris->SetGravityCoeff(GravityCoeffDebris);
			debris->SetVelocity(vel);
			_dynamicObjects.push_back(debris);
		}
	}
}

void GameWidget::Draw() {

	//draw background
	Render::device.PushMatrix();
	_backgroundTexture->Draw();
	Render::device.PopMatrix();	

	//draw dynamic objects
	Render::device.PushMatrix();

	Render::device.MatrixTranslate(_camera.x, 0, 0);

	_map.Draw();

	for (size_t i = 0; i < _debris.size(); ++i) {
		_debris[i]->Draw();
	}

	for (auto d : _dynamicObjects) {
		d->Draw();
	}

	_mouseAnimate.Draw();

	_player->Draw();

	//draw exit
	Render::device.PushMatrix();
	Render::device.MatrixTranslate((_map.GetMapSize().x - 2) * _map.GetTileSize().x, 0, 0.f);
	_exitTexture->Draw();
	Render::device.PopMatrix();

	Render::device.PopMatrix();	

	//draw barO2
	Render::device.PushMatrix();
	Render::device.MatrixRotate(math::Vector3(0, 0, 1), 40);
	Render::device.MatrixTranslate(72, -75, 0);
	_barO2bgTexture->Draw();
	Render::device.PopMatrix();

	Render::device.PushMatrix();
	float length = _player->GetOxygen();
	IRect texRect = _barO2Texture->getBitmapRect();
	FRect texPos = FRect(texRect.LeftBottom().x, texRect.RightBottom().x,
		texRect.LeftBottom().y, texRect.LeftTop().y * float(length / 100));
	FRect uv(0, 1, 0, 1);
	Render::device.MatrixRotate(math::Vector3(0, 0, 1), 40);
	Render::device.MatrixTranslate(72, -75, 0);	
	_barO2Texture->Bind();
	Render::DrawQuad(texPos, uv);
	Render::device.PopMatrix();	

	//draw interface
	Render::device.PushMatrix();
	_interfaceTexture->Draw();
	Render::device.PopMatrix();

	//debug
	/*Render::BindFont("arial");
	Render::PrintString(924 + 100 / 2, 35, utils::lexical_cast(_player->GetOxygen()), 1.f, CenterAlign);
	Render::BindFont("arial");
	Render::PrintString(924 + 100 / 2, 55, utils::lexical_cast((_map.GetMapSize().x - 1) * _map.GetTileSize().x), 1.f, CenterAlign);*/
}

void GameWidget::Update(float dt) {

	_mouseAnimate.Update(dt);

	_timerMusic += dt;
	if (_timerMusic >= 32) {
		MM::manager.PlaySample("Ambient5");
		_timerMusic = 0.f;
	}

	if (_isDeleteBonus && !_world->IsLocked()) {
		for (size_t i = 0; i < _dynamicObjects.size(); ++i) {
			if (_dynamicObjects[i]->GetBody()->GetUserData() == "delete") {
				_dynamicObjects.erase(_dynamicObjects.begin() + i);
				_isDeleteBonus = false;
				break;
			}
		}
	}	

	for (size_t i = 0; i < _debris.size(); ++i) {
		_debris[i]->Update(dt);
	}
	for (auto d : _dynamicObjects) {
		d->Update(dt);
	}
	
	_player->Update(dt);


	float timeStep = 1.f / 60.f;
	_world->Step(dt, 6, 2);

	float coeef = 200.f;
	//camera
	if (_player->GetPosition().x < Render::device.Width() / 2.f && _camera.x >= 0.f) {
		_camera.x = 0.f;
	}
	else if (_player->GetPosition().x > _map.GetMapSize().x * _map.GetTileSize().x - Render::device.Width() / 2.f + coeef) {
		_camera.x = (-(_map.GetMapSize().x * _map.GetTileSize().x - Render::device.Width()));
	}
	else if (_player->GetPosition().x > -_camera.x + Render::device.Width() / 2.f + coeef) {
		_camera.x = (-_player->GetPosition().x + coeef + Render::device.Width() / 2.f);
	}
	else if (_player->GetPosition().x < -_camera.x + Render::device.Width() / 2.f - coeef) {
		_camera.x = (-_player->GetPosition().x - coeef + Render::device.Width() / 2.f);
	}
	if (_player->GetPosition().x >= (_map.GetMapSize().x - 1) * _map.GetTileSize().x) {
		Core::mainScreen.pushLayer("MenuLayer");
	}
}

bool GameWidget::MouseDown(const IPoint& mousePos) {

	if (_player->IsDeath()) {
		Restart();
		return true;
	}

	_isMouseClick = true;
	_idSample = MM::manager.PlaySample("Gravity");

	IPoint mouse_pos = IPoint(-_camera.x + mousePos.x, mousePos.y);
	if (Core::mainInput.GetMouseRightButton()) {
		for (size_t i = 0; i < _debris.size(); ++i) {
			_debris[i]->MouseDownRight(mouse_pos);
		}
		for (auto d : _dynamicObjects) {
			d->MouseDownRight(mouse_pos);
		}

		_mouseAnimate.MouseDownRight(mouse_pos);

		_player->MouseDownRight(mouse_pos);				
	}
	if (Core::mainInput.GetMouseLeftButton()) {
		for (size_t i = 0; i < _debris.size(); ++i) {
			_debris[i]->MouseDownLeft(mouse_pos);
		}
		for (auto d : _dynamicObjects) {
			d->MouseDownLeft(mouse_pos);
		}
		_mouseAnimate.MouseDownLeft(mouse_pos);

		_player->MouseDownLeft(mouse_pos);
	}
	return true;
}

void GameWidget::MouseUp(const IPoint& mousePos) {

	_isMouseClick = false;
	MM::manager.StopSample(_idSample);
	IPoint mouse_pos = IPoint(-_camera.x + mousePos.x, mousePos.y);
	for (size_t i = 0; i < _debris.size(); ++i) {
		_debris[i]->MouseUp(mouse_pos);
	}
	for (auto d : _dynamicObjects) {
		d->MouseUp(mouse_pos);
	}
	_player->MouseUp(mouse_pos);

	_mouseAnimate.MouseUp(mouse_pos);
}

void GameWidget::AcceptMessage(const Message& message) {
	if (message.getPublisher() == "Powerup") {
		_isDeleteBonus = true;
		_player->FillOxygen();
		MM::manager.PlaySample("Oxygen");
	}
	else if (message.getPublisher() == "RepairKit") {
		_isDeleteBonus = true;
		_player->FillHealth();
		MM::manager.PlaySample("Scotch");
	}
	else if (message.getPublisher() == "PlayerCollision") {
		_player->Collision();
	}

	if (message.getPublisher() == "Restart") {
		Restart();
	}
}

std::vector<b2Vec2> GetVertexShape(const std::string& nameTex) {

	std::vector<b2Vec2> vertex;
	if (nameTex == "Box") {
		vertex.push_back(b2Vec2(0, 112));
		vertex.push_back(b2Vec2(20, 4));
		vertex.push_back(b2Vec2(140, 0));
		vertex.push_back(b2Vec2(168, 64));
		vertex.push_back(b2Vec2(158, 162));
		vertex.push_back(b2Vec2(46, 166));
	}
	else if (nameTex == "Blocks") {
		vertex.push_back(b2Vec2(0, 51));
		vertex.push_back(b2Vec2(5, 2));
		vertex.push_back(b2Vec2(180, 21));
		vertex.push_back(b2Vec2(175, 63));
		vertex.push_back(b2Vec2(150, 101));
		vertex.push_back(b2Vec2(76, 121));
		vertex.push_back(b2Vec2(22, 109));
	}
	/*else if (nameTex == "Barrel") {
		vertex.push_back(b2Vec2(0, 110));
		vertex.push_back(b2Vec2(6, 9));
		vertex.push_back(b2Vec2(68, 1));
		vertex.push_back(b2Vec2(124, 9));
		vertex.push_back(b2Vec2(130, 110));
		vertex.push_back(b2Vec2(123, 159));
		vertex.push_back(b2Vec2(70, 165));
		vertex.push_back(b2Vec2(6, 158));
	}*/
	else if(nameTex == "Criogen") {
		vertex.push_back(b2Vec2(6, 8));
		vertex.push_back(b2Vec2(110, 8));
		vertex.push_back(b2Vec2(107, 130));
		vertex.push_back(b2Vec2(74, 154));
		vertex.push_back(b2Vec2(40, 154));
		vertex.push_back(b2Vec2(10, 137));
	}
	return vertex;
}

b2Vec2 RandomVelocity() {
	float factor = 1000.f;
	Config& instance = Config::GetInstance();
	float MaxStartSpeed = instance.GetConstantsByKey("MaxStartSpeed") * factor;
	float velX = (rand() % int(MaxStartSpeed * 2 / sqrt(2)) - (MaxStartSpeed / sqrt(2))) / factor;
	float velY = (rand() % int(MaxStartSpeed * 2 / sqrt(2)) - (MaxStartSpeed / sqrt(2))) / factor;
	return b2Vec2(velX, velY);
}
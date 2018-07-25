#include "stdafx.h"
#include "Player.h"
#include "Config.h"

using namespace variable;

Player::Player() 
	: Node()
{	
	Init();
}

Player::Player(const FPoint& pos, const std::string& nameText,
	float density, float restitution, Node::TypeShape type) 
	: Node(pos, nameText, density, restitution, type)
{	
	Init();
	_gravityComponent = GravityComponent(_body, 1.f);
}

void Player::Init() {

	_flashAnimation.reset(new FlashAnimation("astroman", "ASTROMAN"));
	_flashAnimation->Reset();

	Config& instance = Config::GetInstance();
	_maxSpeed = instance.GetConstantsByKey("MaxPlayerSpeed") * SCALE;

	_text = Core::resourceManager.Get<Render::Texture>("helm0");
}

void Player::MouseDownLeft(const IPoint& mouse_pos) {
	_gravityComponent.ApplyForceRepellent(mouse_pos);
}

void Player::MouseDownRight(const IPoint& mouse_pos) {
	_gravityComponent.ApplyForceGravity(mouse_pos);
}

void Player::MouseUp(const IPoint& mouse_pos) {
	_gravityComponent.StopForce();
}

void Player::Draw() {

	if (_flashAnimation == nullptr) {
		Node::Draw();
	}

	if (_flashAnimation != nullptr) {
		Render::device.PushMatrix();
		b2Vec2 pos = _body->GetPosition();

		Render::device.MatrixTranslate(pos.x * SCALE, pos.y * SCALE, 0.f);
		Render::device.MatrixRotate(math::Vector3(0, 0, 1), _body->GetAngle() * DEG);

		_flashAnimation->Draw(0, 0);
		Render::device.PopMatrix();
	}

	if (_text != nullptr) {
		Render::device.PushMatrix();
		b2Vec2 pos = _body->GetPosition();
		IRect texRect = _texture->getBitmapRect();
		FRect rect(texRect);
		FRect uv(0, 1, 0, 1);

		Render::device.MatrixTranslate(pos.x * SCALE, pos.y * SCALE, 0.f);
		Render::device.MatrixRotate(math::Vector3(0, 0, 1), _body->GetAngle() * DEG);
			
		_text->TranslateUV(rect, uv);
		_text->Bind();

		Render::device.MatrixTranslate(-texRect.width * 0.5f, -texRect.height * 0.5f, 0.f);
		Render::DrawQuad(rect, uv);
		Render::device.PopMatrix();
	}

	Render::BindFont("arial");
	Render::PrintString(924 + 100 / 2, 55, utils::lexical_cast(abs(_body->GetLinearVelocity().Length())), 1.f, CenterAlign);

	Render::BindFont("arial");
	Render::PrintString(924 + 100 / 2, 35, utils::lexical_cast(_health), 1.f, CenterAlign);
}

void Player::Update(float dt) {

	if (!_isDeath && _isAnimation && _timeAnimation <= 0.f) 
	{
		_isAnimation = false;
		_flashAnimation.reset(new FlashAnimation("astroman", "ASTROMAN"));
		_flashAnimation->Reset();
	}
	else if (_isDeath && _isAnimation && _flashAnimation->GetCurrentFrame() == _flashAnimation->GetLastFrame()) {
		_isAnimation = false;
		_flashAnimation = nullptr;
		_texture = Core::resourceManager.Get<Render::Texture>("ASTROMAN_end");
		_isRestart = true;
	}
	else if (_isAnimation) {
		_timeAnimation -= dt;
	}

	if (_flashAnimation != nullptr) {
		_flashAnimation->Update(dt);
	}

	_oxygen -= dt;

	if (!_isDeath && _oxygen <= 0) {
		_isDeath = true;
		_texture = Core::resourceManager.Get<Render::Texture>("ASTROMAN_end_O2");
		_text = nullptr;
		_flashAnimation = nullptr;
		_isRestart = true;
	}

	auto length = _body->GetLinearVelocity().Length();
	if (length > _maxSpeed) {
		auto velocity = _body->GetLinearVelocity();
		float x = velocity.x;
		float y = velocity.y;
		float k = (x * x + y * y) / (_maxSpeed * _maxSpeed);
		float newX = x / (sqrt(k));
		float newY = y / (sqrt(k));
		_body->SetLinearVelocity(b2Vec2(newX, newY));
	}

	_gravityComponent.Update(dt);
}

void Player::Collision() {

	if (_isAnimation || _isDeath) {
		return;
	}

	int hit = rand() % 3 + 1;
	MM::manager.PlaySample("Hit" + std::to_string(hit));
	_isAnimation = true;
	_flashAnimation.reset(new FlashAnimation("astroman", "ASTROMAN_PANIC"));
	_flashAnimation->Reset();
	_timeAnimation = 0.8f;
	float length = _body->GetLinearVelocity().Length();
	if (abs(_body->GetLinearVelocity().Length()) > 1.5f) {
		_health -=  length * 3;
	}
	UpdateSuit();
}

void Player::UpdateSuit() {
	if (_health > 79 && _health < 90) {
		_text = Core::resourceManager.Get<Render::Texture>("helm1");
	}
	else if(_health > 59 && _health < 79) {
		_text = Core::resourceManager.Get<Render::Texture>("helm2");
	}
	else if(_health > 39 && _health < 59) {
		_text = Core::resourceManager.Get<Render::Texture>("helm3");
	}
	else if(_health > 0 && _health < 19) {
		_text = Core::resourceManager.Get<Render::Texture>("helm4");
	}
	else if (_health <= 0) {
		_text = nullptr;
		_flashAnimation.reset(new FlashAnimation("astroman", "ASTROMAN_DEATH"));
		_flashAnimation->Reset();
		_isAnimation = true;
		_isDeath = true;
	}
}

void Player::FillHealth() {
	_health += 30;
	if (_health > 100) {
		_health = 100;
	}
	UpdateSuit();
}

void Player::FillOxygen() {
	_oxygen = 100;
}

Player::~Player() {

}
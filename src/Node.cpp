#include "stdafx.h"
#include "Node.h"
#include "Config.h"

using variable::SCALE;
using variable::DEG;

Node::Node() {}

Node::Node(const FPoint& pos, const std::string& nameText, 
	float density, float restitution, TypeShape type) 
{
	_texture = Core::resourceManager.Get<Render::Texture>(nameText);

	Config& instance = Config::GetInstance();
	auto world = instance.GetWorld();

	b2BodyDef bodyDef;
	bodyDef.position.Set(pos.x / SCALE, pos.y / SCALE);
	bodyDef.type = b2_dynamicBody;
	_body = world->CreateBody(&bodyDef);

	InitShape(type);

	b2FixtureDef fixDef;
	fixDef.density = density;
	fixDef.restitution = restitution;
	fixDef.shape = _shape;
	_body->CreateFixture(&fixDef);
}

Node::Node(const FPoint& pos, const std::string& nameTex,
	float density, float restitution, const std::vector<b2Vec2>& vertex)
	: _isPolygon(true)
{

	b2Vec2 * vs = new b2Vec2[vertex.size()];

	for (size_t i = 0; i < vertex.size(); ++i) {
		vs[i] = b2Vec2(vertex[i].x / SCALE, vertex[i].y / SCALE);
	}

	_texture = Core::resourceManager.Get<Render::Texture>(nameTex);

	Config& instance = Config::GetInstance();
	auto world = instance.GetWorld();

	b2BodyDef bodyDef;
	bodyDef.position.Set(pos.x / SCALE, pos.y / SCALE);
	bodyDef.type = b2_dynamicBody;
	_body = world->CreateBody(&bodyDef);

	b2PolygonShape sh;
	sh.Set(vs, vertex.size());
	_shape = new b2PolygonShape(sh);

	b2FixtureDef fixDef;
	fixDef.density = density;
	fixDef.restitution = restitution;
	fixDef.shape = _shape;
	_body->CreateFixture(&fixDef);

	delete[] vs;	
}

void Node::Draw() {
	
	if (_isPolygon) {
		Render::device.PushMatrix();

		b2Vec2 pos = _body->GetPosition();

		Render::device.MatrixTranslate(pos.x * SCALE, pos.y * SCALE, 0.f);
		Render::device.MatrixRotate(math::Vector3(0, 0, 1), _body->GetAngle() * DEG);

		_texture->Draw();

		Render::device.PopMatrix();
	}
	else {
		Render::device.PushMatrix();

		b2Vec2 pos = _body->GetPosition();
		IRect texRect = _texture->getBitmapRect();
		FRect rect(texRect);
		FRect uv(0, 1, 0, 1);

		Render::device.MatrixTranslate(pos.x * SCALE, pos.y * SCALE, 0.f);
		Render::device.MatrixRotate(math::Vector3(0, 0, 1), _body->GetAngle() * DEG);

		_texture->TranslateUV(rect, uv);
		_texture->Bind();

		Render::device.MatrixTranslate(-texRect.width * 0.5f, -texRect.height * 0.5f, 0.f);
		Render::DrawQuad(rect, uv);

		Render::device.PopMatrix();
	}
}

void Node::InitShape(TypeShape type) {
	if (type == TypeShape::Box) {
		float width = _texture->getBitmapRect().width / 2.f;
		float height = _texture->getBitmapRect().height / 2.f;
		b2PolygonShape sh;
		sh.SetAsBox(width / SCALE, height / SCALE);
		_shape = new b2PolygonShape(sh);
	}
	else if (type == TypeShape::Circle) {
		float radius = _texture->getBitmapRect().width / 2.f - 5.f;
		b2CircleShape sh;
		sh.m_radius = radius / SCALE;
		_shape = new b2CircleShape(sh);
	}
}

Node::~Node() {
	b2World* world = _body->GetWorld();
	delete _shape;
	if (!world->IsLocked()) {
		world->DestroyBody(_body);
	}
}

void Node::Update(float dt) {}
void Node::MouseDownLeft(const IPoint& mouse_pos) {}
void Node::MouseDownRight(const IPoint& mouse_pos) {}
void Node::MouseUp(const IPoint& mouse_pos) {}

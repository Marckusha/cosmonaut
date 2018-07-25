#include "stdafx.h"
#include "MouseAnimate.h"

MouseAnimate::MouseAnimate() {}

MouseAnimate::MouseAnimate(const IPoint& pos) 
	: _position(pos) 
{}


void MouseAnimate::Draw() {
	if (_flashAnimation != nullptr && _isMouseClick) {
		Render::device.PushMatrix();

		Render::BeginAlphaMul(_timer);
		_flashAnimation->Draw(_position.x, _position.y);
		Render::EndAlphaMul();

		Render::device.PopMatrix();
	}
}

void MouseAnimate::Update(float dt) {
	if (_flashAnimation != nullptr && _isMouseClick) {
		_flashAnimation->Update(dt);
	}

	if (!_isMouseUp && _isMouseClick && _timer < 1.f) {
		_timer += dt;
		if (_timer >1.f) {
			_timer = 1.f;
		}
	}
 
	if (_isMouseUp) {
		_timer -= dt;
		if (_timer <= 0) {
			_isMouseClick = false;
			_isMouseUp = false;
		}
	}
}

void MouseAnimate::MouseDownLeft(const IPoint& mouse_pos) {
	_isMouseClick = true;
	_isMouseUp = false;
	_flashAnimation.reset(new FlashAnimation("gravity_in_out", "gravity_out"));
	_flashAnimation->Reset();
	_position = mouse_pos;
	_timer = 0;
}

void MouseAnimate::MouseDownRight(const IPoint& mouse_pos) {
	_isMouseClick = true;
	_isMouseUp = false;
	_flashAnimation.reset(new FlashAnimation("gravity_in_out", "gravity_in"));
	_flashAnimation->Reset();
	_position = mouse_pos;
	_timer = 0;
}

void MouseAnimate::MouseUp(const IPoint& mouse_pos) {
	_isMouseUp = true;
}
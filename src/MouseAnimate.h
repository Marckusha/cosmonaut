#pragma once

class MouseAnimate {
public:
	MouseAnimate();
	MouseAnimate(const IPoint& pos);

	void Draw();
	void Update(float dt);

	void MouseDownLeft(const IPoint& mouse_pos);
	void MouseDownRight(const IPoint& mouse_pos);
	void MouseUp(const IPoint& mouse_pos);

private:
	FlashAnimationPtr _flashAnimation = nullptr;
	IPoint _position = IPoint();
	bool _isMouseClick = false;
	float _timer = 0.f;
	bool _isMouseUp = false;
};
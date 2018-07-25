#include "stdafx.h"
#include "MenuWidget.h"

MenuWidget::MenuWidget(const std::string& name, rapidxml::xml_node<>* elem) 
	: Widget(name)
{
	Init();
}

void MenuWidget::Init() {
	_texture = Core::resourceManager.Get<Render::Texture>("Tutorial");
}

void MenuWidget::Draw() {
	Render::device.PushMatrix();

	_texture->Draw();

	Render::device.PopMatrix();
}

void MenuWidget::Update(float dt) {

}

void MenuWidget::AcceptMessage(const Message& message) {
	if (message.getPublisher() == "GameWin") {
		_texture = Core::resourceManager.Get<Render::Texture>("Win");
	}
}

bool MenuWidget::MouseDown(const IPoint& mouse_pos) {
	Core::mainScreen.popLayer();
	if (!_isFirstGame) {
		Message message("Restart");
		Core::mainScreen.GetTopLayer()->getWidget("GameWidget")->AcceptMessage(message);
	}
	else {
		_isFirstGame = false;
		_texture = Core::resourceManager.Get<Render::Texture>("Win");
	}
	return true;
}
void MenuWidget::MouseUp(const IPoint& mouse_pos) {

}
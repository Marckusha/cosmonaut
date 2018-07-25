#pragma once

class MenuWidget : public GUI::Widget
{
public:
	MenuWidget(const std::string& name, rapidxml::xml_node<>* elem);

	void Draw() override;
	void Update(float dt) override;

	void AcceptMessage(const Message& message) override;

	bool MouseDown(const IPoint& mouse_pos) override;
	void MouseUp(const IPoint& mouse_pos) override;

private:
	void Init();
	
private:
	Render::Texture* _texture = nullptr;
	bool _isFirstGame = true;
};

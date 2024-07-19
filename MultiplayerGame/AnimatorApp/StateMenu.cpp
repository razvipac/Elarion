#include "StateMenu.h"
#include "Animator.h"
using namespace std;
using namespace sf;
//	InputField(const sf::Vector2f& size, const sf::Vector2f& position, const std::string& text, const std::function<void(const std::string&)>& onTextChange);

StateMenu::StateMenu(const Vector2f& size, const Vector2f& position, const string& text) : inputField(Vector2f(100, 20), Vector2f(position.x + 10, position.y + 40), "State Name")
{
	shape.setSize(size);
	shape.setPosition(position);
	shape.setFillColor(Color::White);
	shape.setOutlineColor(Color::Black);
	shape.setOutlineThickness(1);
	this->text.setFont(font);
	this->text.setString(text);
	this->text.setCharacterSize(20);
	this->text.setFillColor(Color::Black);
	this->text.setPosition(position.x + 10, position.y + 10);
	inputField.setOnFinishEdit([this](const string& text) {
		if(Animator::getSelectedState() != nullptr)
			Animator::getSelectedState()->setName(text);
	});
}

StateMenu& StateMenu::getInstance()
{
	static StateMenu instance(Vector2f(200, 100), Vector2f(200, 200), "State Name");
	return instance;
}

void StateMenu::draw(RenderWindow& window) const
{
	window.draw(shape);
	window.draw(text);
	inputField.draw(window);
}

void StateMenu::handleEvent(const Event& event)
{
	inputField.handleEvent(event);
}

void StateMenu::setText(const string& text)
{
	this->text.setString(text);
}

const string& StateMenu::getText() const
{
	return text.getString();
}
#include "Button.h"
using namespace std;
using namespace sf;
Button::Button(const Vector2f& size, const Vector2f& position, const string& text, const function<void()>& onClick) : onClick(onClick)
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
}
void Button::draw(RenderWindow& window) const
{
	window.draw(shape);
	window.draw(text);
}
void Button::handleEvent(const Event& event)
{
	if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
	{
		Vector2f mousePosition = Vector2f(event.mouseButton.x, event.mouseButton.y);
		if (shape.getGlobalBounds().contains(mousePosition))
		{
			onClick();
		}
	}
}
void Button::setText(const string& text)
{
	this->text.setString(text);
}
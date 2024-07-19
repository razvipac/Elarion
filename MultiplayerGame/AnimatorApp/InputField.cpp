#include "InputField.h"
using namespace std;
using namespace sf;
InputField::InputField(const Vector2f& size, const Vector2f& position, const string& text, const function<void(const string&)>& onTextChange) : onTextChange(onTextChange)
{
	shape.setSize(size);
	shape.setPosition(position);
	shape.setFillColor(Color::White);
	shape.setOutlineColor(Color::Black);
	shape.setOutlineThickness(1);
	font.loadFromFile("Resources/Roboto-Black.ttf");
	this->text.setFont(font);
	this->text.setString(text);
	this->text.setCharacterSize(20);
	this->text.setFillColor(Color::Black);
	this->text.setPosition(position.x + 5, position.y + 5);
	isSelected = false;
}
void InputField::draw(RenderWindow& window) const
{
	window.draw(shape);
	window.draw(text);
}
void InputField::handleEvent(const Event& event)
{
	if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
	{
		Vector2f mousePosition = Vector2f(event.mouseButton.x, event.mouseButton.y);
		if (shape.getGlobalBounds().contains(mousePosition))
		{
			isSelected = true;
		}
		else
		{
			isSelected = false;
		}
	}
	else if (event.type == Event::TextEntered && isSelected)
	{
		if (event.text.unicode == 8 && text.getString().getSize() > 0)
		{
			string currentText = text.getString();
			currentText.pop_back();
			text.setString(currentText);
			onTextChange(currentText);
		}
		else if (event.text.unicode >= 32 && event.text.unicode <= 126)
		{
			string currentText = text.getString();
			currentText += event.text.unicode;
			text.setString(currentText);
			onTextChange(currentText);
		}
	}
}
void InputField::setText(const string& text)
{
	this->text.setString(text);
}
#include "InputField.h"
using namespace std;
using namespace sf;
InputField::InputField(const Vector2f& size, const Vector2f& position, const string& text)
{
	shape.setSize(size);
	shape.setPosition(position);
	shape.setFillColor(Color::White);
	shape.setOutlineColor(Color::Black);
	shape.setOutlineThickness(1);
	this->text.setFont(font);
	this->text.setString(text);
	this->text.setCharacterSize(17);
	this->text.setFillColor(Color::Black);
	this->text.setPosition(position.x + 6, position.y + 6);
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
		if (shape.getGlobalBounds().contains(mousePositionInUI) && !clickUsed)
		{
			clickUsed = true;
			isSelected = true;
			shape.setOutlineColor(Color::Green);
		}
		else if (isSelected)
		{
			isSelected = false;
			shape.setOutlineColor(Color::Black);
			if(onFinishEdit != nullptr)
				onFinishEdit(text.getString());
		}
	}
	else if (event.type == Event::TextEntered && isSelected)
	{
		if (event.text.unicode == 8 && text.getString().getSize() > 0)
		{
			string currentText = text.getString();
			currentText.pop_back();
			text.setString(currentText);
		}
		else if (event.text.unicode >= 32 && event.text.unicode <= 126)
		{
			string currentText = text.getString();
			currentText += event.text.unicode;
			text.setString(currentText);
		}
	}
	else if (event.type == Event::KeyPressed && event.key.code == Keyboard::Enter && isSelected)
	{
		isSelected = false;
		shape.setOutlineColor(Color::Black);
		if(onFinishEdit != nullptr)
			onFinishEdit(text.getString());
	}
}
void InputField::setText(const string& text)
{
	this->text.setString(text);
}
const Text& InputField::getText() const
{
	return text;
}
void InputField::setOnFinishEdit(const function<void(const std::string&)>& onFinishEdit)
{
	this->onFinishEdit = onFinishEdit;
}

void InputField::setPosition(const Vector2f& position)
{
	shape.setPosition(position);
	text.setPosition(position.x + 6, position.y + 6);
}

const std::string& InputField::getValue() const {
	return text.getString();
}

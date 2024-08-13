#include "Bar.h"

using namespace std;
using namespace sf;

Bar::Bar()
{
	fill.setSize(Vector2f(36, 10));
	background.setSize(Vector2f(36, 10));
	value = 50;
	maxValue = 100;
	fill.setFillColor(Color::Green);
	background.setFillColor(Color::Red);

	fill.setPosition(0, 0);
	background.setPosition(0, 0);

	background.setOutlineColor(Color(100, 100, 100));
	background.setOutlineThickness(2);

	targetValue = value;
	smoothSpeed = 0.7f;
} 
void Bar::update(float deltaTime)
{
	fill.setSize(Vector2f((value / maxValue) * background.getSize().x, background.getSize().y));
	if (value < targetValue)
	{
		value += smoothSpeed * deltaTime * maxValue;
		if (value > targetValue)
			value = targetValue;
	}
	else if (value > targetValue)
	{
		value -= smoothSpeed * deltaTime * maxValue;
		if (value < targetValue)
			value = targetValue;
	}
}
void Bar::draw(RenderWindow& window) const
{
	window.draw(background);
	window.draw(fill);
}
void Bar::setValue(float value)
{
	targetValue = value;
}
void Bar::setMaxValue(float maxValue)
{
	this->maxValue = maxValue;
}
float Bar::getValue() const
{
	return targetValue;
}
float Bar::getMaxValue() const
{
	return maxValue;
}
void Bar::setPosition(const Vector2f& position)
{
	fill.setPosition(position);
	background.setPosition(position);
}
void Bar::setSize(const Vector2f& size)
{
	fill.setSize(size);
	background.setSize(size);
}
void Bar::setFillColor(const Color& color)
{
	fill.setFillColor(color);
}
void Bar::setBackgroundColor(const Color& color)
{
	background.setFillColor(color);
}
void Bar::setFillPercentage(float fillPercentage)
{
	value = maxValue * fillPercentage;
}
float Bar::getFillPercentage()
{
	return value / maxValue;
}
const Vector2f& Bar::getPosition() const
{
	return fill.getPosition();
}
const Vector2f& Bar::getSize() const
{
	return fill.getSize();
}
const Color& Bar::getFillColor() const
{
	return fill.getFillColor();
}
const Color& Bar::getBackgroundColor() const
{
	return background.getFillColor();
}
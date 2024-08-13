#pragma once
#include "SFML/Graphics.hpp"

class Bar {
private:
	sf::RectangleShape fill;
	sf::RectangleShape background;
	float value;
	float maxValue;
	float targetValue;
	float smoothSpeed;
public:
	Bar();
	void update(float delaTime);
	void draw(sf::RenderWindow& window) const;
	void setValue(float value);
	void setMaxValue(float maxValue);
	float getValue() const;
	float getMaxValue() const;
	void setPosition(const sf::Vector2f& position);
	void setSize(const sf::Vector2f& size);
	void setFillColor(const sf::Color& color);
	void setBackgroundColor(const sf::Color& color);
	void setFillPercentage(float fillPercentage);
	float getFillPercentage();
	const sf::Vector2f& getPosition() const;
	const sf::Vector2f& getSize() const;
	const sf::Color& getFillColor() const;
	const sf::Color& getBackgroundColor() const;
};
#pragma once
#include "SFML/Graphics.hpp"
#include <functional>
#include <iostream>

extern bool clickUsed;

extern sf::Font font;

extern sf::Vector2f mousePositionInUI;

class Button
{
private:
	sf::RectangleShape shape;
	sf::Text text;
	std::function<void()> onClick;
public:
	Button(const sf::Vector2f& size, const sf::Vector2f& position, const std::string& text, const std::function<void()>& onClick);
	void draw(sf::RenderWindow& window) const;
	void handleEvent(const sf::Event& event);
	void setText(const std::string& text);
	void setOnClick(const std::function<void()>& onClick);
	void setPosition(const sf::Vector2f& position);
};
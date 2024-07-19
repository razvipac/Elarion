#pragma once
#include "SFML/Graphics.hpp"
#include <functional>
#include <iostream>

class Button
{
private:
	sf::RectangleShape shape;
	sf::Font font;
	sf::Text text;
	std::function<void()> onClick;
public:
	Button(const sf::Vector2f& size, const sf::Vector2f& position, const std::string& text, const std::function<void()>& onClick);
	void draw(sf::RenderWindow& window) const;
	void handleEvent(const sf::Event& event);
	void setText(const std::string& text);
};
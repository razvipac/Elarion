#pragma once
#include "SFML/Graphics.hpp"
#include <functional>
#include <iostream>
class InputField
{
private:
	sf::RectangleShape shape;
	sf::Font font;
	sf::Text text;
	bool isSelected;
	std::function<void(const std::string&)> onTextChange;
public:
	InputField(const sf::Vector2f& size, const sf::Vector2f& position, const std::string& text, const std::function<void(const std::string&)>& onTextChange);
	void draw(sf::RenderWindow& window) const;
	void handleEvent(const sf::Event& event);
	void setText(const std::string& text);
};
#pragma once
#include "SFML/Graphics.hpp"
#include <functional>
#include <iostream>

extern bool clickUsed;

extern sf::Font font;

extern sf::Vector2f mousePositionInUI;

class InputField
{
private:
	sf::RectangleShape shape;
	sf::Text text;
	bool isSelected;
	std::function<void(const std::string&)> onFinishEdit;
public:
	InputField(const sf::Vector2f& size, const sf::Vector2f& position, const std::string& text);
	void draw(sf::RenderWindow& window) const;
	void handleEvent(const sf::Event& event);
	void setText(const std::string& text);
	const sf::Text& getText() const;
	void setOnFinishEdit(const std::function<void(const std::string&)>& onFinishEdit);
};
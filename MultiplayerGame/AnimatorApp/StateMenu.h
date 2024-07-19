#pragma once
#include "SFML/Graphics.hpp"
#include "InputField.h"

extern sf::Font font;

class StateMenu
{
private:
	sf::RectangleShape shape;
	sf::Text text;
	InputField inputField;
	StateMenu(const sf::Vector2f& size, const sf::Vector2f& position, const std::string& text);
public:
	StateMenu& operator=(const StateMenu&) = delete;
	StateMenu(const StateMenu&) = delete;
	static StateMenu& getInstance();
	void draw(sf::RenderWindow& window) const;
	void handleEvent(const sf::Event& event);
	void setText(const std::string& text);
	const std::string& getText() const;
};
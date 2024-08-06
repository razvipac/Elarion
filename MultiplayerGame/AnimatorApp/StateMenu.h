#pragma once
#include "SFML/Graphics.hpp"
#include "InputField.h"
#include "Button.h"
#include "Animator.h"

extern sf::Font font;

class StateMenu
{
private:
	sf::RectangleShape shape;
	sf::Text text;
	//InputField inputField;
	Button addStateButton;
	Animator* animator;
	Button addTransitionButton;
	Button saveButton;
	Button loadButton;
	Button setDefaultButton;
	StateMenu(const sf::Vector2f& size, const sf::Vector2f& position);
public:
	StateMenu& operator=(const StateMenu&) = delete;
	StateMenu(const StateMenu&) = delete;
	static StateMenu& getInstance();
	void draw(sf::RenderWindow& window) const;
	void handleEvent(const sf::Event& event);
	void setText(const std::string& text);
	std::string getText() const;
	void setAnimator(Animator* animator);
};
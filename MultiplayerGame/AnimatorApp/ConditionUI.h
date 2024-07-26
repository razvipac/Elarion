#pragma once
#include "SFML/Graphics.hpp"
#include "Button.h"
#include "InputField.h"

class ConditionUI
{
private:
	sf::Vector2f position;
	sf::Text text;
	sf::Text variableName;
	InputField variableNameInputField;
	sf::Text operatorText;
	InputField operatorInputField;
	sf::Text valueText;
	InputField valueInputField;
	sf::Text type;
	InputField typeInputField;
	Button removeButton;
public:
	ConditionUI(const sf::Vector2f& position, int index);
	void draw(sf::RenderWindow& window) const;
	void handleEvent(const sf::Event& event);
	void setPosition(const sf::Vector2f& position);
	void setIndex(int index);
};
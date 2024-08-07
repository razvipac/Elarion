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

	std::string nameString;
	std::string operatorString;
	std::string valueString;
	std::string typeString;
public:
	ConditionUI(const sf::Vector2f& position, int index);
	void draw(sf::RenderWindow& window) const;
	void handleEvent(const sf::Event& event);
	void setPosition(const sf::Vector2f& position);
	const sf::Vector2f& getPosition() const;
	void setIndex(int index);
	void setValues(const std::string& variableName, int operatorIndex, const std::string& value, bool isFloat);
	int getTypeFromOperator(const std::string& operatorString) const;
	void updateCondition() const;
	~ConditionUI();
};
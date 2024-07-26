#pragma once
#include "SFML/Graphics.hpp"
#include <vector>
#include <iostream>

extern bool clickUsed;
extern sf::Vector2f mousePositionInWorld;
extern bool isAddingTransition;

struct FloatingBool {
	bool isFloat;
	bool boolValue;
	float floatValue;
	FloatingBool(bool value) : isFloat(false), boolValue(value) {}
	FloatingBool(float value) : isFloat(true), floatValue(value) {}
};

class State;
class Transition
{
private:
	State& startingState;
	State& arrivalState;
	sf::Vector2f offset;
	sf::RectangleShape shape;
	sf::ConvexShape arrow;
	bool mouseIntersectsLine() const;
	std::vector<std::string> names;
	std::vector<int> operators; // 0 = ==, 1 = !=, 2 = <, 3 = >, 4 = <=, 5 = >=
	std::vector<FloatingBool> values;

public:
	Transition(State& startingState, State& arrivalState);
	const State& getArrivalState() const;
	void draw(sf::RenderWindow& window) const;
	void handleEvent(const sf::Event& event);
	void updatePositionAndRotation();
	void setOutline(bool outline);
	void addCondition(const std::string& name, int operatorIndex, const FloatingBool& value);
	void modifyCondition(int index, const std::string& name, int operatorIndex, const FloatingBool& value);
	void removeCondition(int index);
	int getConditionsCount() const;
	~Transition();
};
#pragma once
#include <vector>
#include <iostream>
#include <string>
#include "SFML/Graphics.hpp"
class State; //Forward declaration

struct FloatingBool {
	bool isFloat;
	bool boolValue;
	float floatValue;
	FloatingBool() : isFloat(false), boolValue(false) {}
	FloatingBool(bool value) : isFloat(false), boolValue(value) {}
	FloatingBool(float value) : isFloat(true), floatValue(value) {}
	std::string toStringLmao() const {
		if (isFloat) {
			return std::to_string(floatValue);
		}
		else {
			return boolValue ? "true" : "false";
		}
	}
};

class Transition {
private:
	State& startingState;
	State& arrivalState;

	std::vector<std::string> names;
	std::vector<int> operators; // 0 = ==, 1 = !=, 2 = <, 3 = >, 4 = <=, 5 = >=
	std::vector<FloatingBool> values;
public:
	Transition(State& startingState, State& arrivalState);
	State& getArrivalState() const;
	int getConditionsCount() const;
	const std::string& getConditionName(int index) const;
	int getConditionOperator(int index) const;
	const FloatingBool& getConditionValue(int index) const;

	//void saveTransition(std::ofstream& file) const;
	void loadTransitionConditions(std::ifstream& file);

	~Transition();
};
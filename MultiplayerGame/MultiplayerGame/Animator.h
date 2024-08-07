#pragma once

#include "State.h"
#include "unordered_map"

class Animator {
private:
	std::vector<State*> states;
	State* currentState;
	int defaultStateIndex;

	std::unordered_map<std::string, FloatingBool> variables;
	sf::Shape& shape;
	void checkTransitions();
public:
	Animator(sf::Shape& shape);
	int getDefaultStateIndex() const;
	const std::vector<State*>& getStates() const;
	State* getCurrentState();

	void setVariable(const std::string& name, const FloatingBool& value);
	void setFloat(const std::string& name, float value);
	void setBool(const std::string& name, bool value);

	void update(float deltaTime);
	const sf::IntRect& getFrame() const;
	void loadAnimator(const std::string& path);
	~Animator();
};
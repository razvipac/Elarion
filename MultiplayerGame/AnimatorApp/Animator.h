#pragma once
#include "State.h"
#include "Button.h"
class Animator
{
private:
	std::vector<State> states;
	int defaultStateIndex;
	Button button;
public:
	Animator();
	void addState();
	void setDefaultStateIndex(int defaultStateIndex);
	int getDefaultStateIndex() const;
	const std::vector<State>& getStates() const;
	void draw(sf::RenderWindow& window) const;
	void handleEvent(const sf::Event& event);
};
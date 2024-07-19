#pragma once
#include "State.h"
#include "Button.h"

extern bool clickUsed;

class Animator
{
private:
	std::vector<State> states;
	static State* selectedState;
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
	static State* getSelectedState();
	static void setSelectedState(State* selectedState);
};
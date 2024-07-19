#include "Animator.h"
#include <iostream>
using namespace sf;
using namespace std;
Animator::Animator() : button(Vector2f(100, 50), Vector2f(0, 0), "Add State", [this]() { addState(); })
{
	defaultStateIndex = -1;
}

void Animator::addState()
{
	states.push_back(State());
}

void Animator::setDefaultStateIndex(int defaultStateIndex)
{
	this->defaultStateIndex = defaultStateIndex;
}
int Animator::getDefaultStateIndex() const
{
	return defaultStateIndex;
}

const vector<State>& Animator::getStates() const
{
	return states;
}
void Animator::draw(sf::RenderWindow& window) const
{
	for (const State& state : states)
	{
		state.draw(window);
	}
}
void Animator::handleEvent(const sf::Event& event)
{
	/*for (State& state : states)
	{
		state.handleEvent(event);
	}*/
}
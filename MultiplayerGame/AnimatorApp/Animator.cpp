#include "Animator.h"
#include "StateMenu.h"
#include <iostream>
using namespace sf;
using namespace std;
State* Animator::selectedState = nullptr;
Animator::Animator() : button(Vector2f(100, 50), Vector2f(0, 0), "Add State", [this]() { addState(); })
{
	defaultStateIndex = -1;
}

void Animator::addState()
{
	states.push_back(State());
	cout << "State added" << endl;
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
	button.draw(window);
}
void Animator::handleEvent(const sf::Event& event)
{
	for (int i=states.size()-1; i>=0; i--)
	{
		states[i].handleEvent(event);
	}
	button.handleEvent(event);
}

State* Animator::getSelectedState()
{
	return selectedState;
}
void Animator::setSelectedState(State* selectedState)
{
	if(Animator::selectedState != nullptr && Animator::selectedState != selectedState)
		Animator::selectedState->deselect();
	Animator::selectedState = selectedState;
	if(selectedState != nullptr)
		StateMenu::getInstance().setText(selectedState->getName());
	else
		StateMenu::getInstance().setText("");
}
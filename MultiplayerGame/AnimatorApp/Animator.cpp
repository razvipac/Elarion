#include "Animator.h"
#include "StateMenu.h"
#include <iostream>
using namespace sf;
using namespace std;
State* Animator::selectedState = nullptr;
Animator::Animator()
{
	defaultStateIndex = -1;
}

void Animator::addState(const string& name, const string& path)
{
	for(int i=0; i<states.size(); i++)
	{
		if(states[i]->getPath() == path)
		{
			cout << "State with that name already exists" << endl;
			return;
		}
	}
	states.push_back(new State(name, path));
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

const vector<State*>& Animator::getStates() const
{
	return states;
}
void Animator::draw(sf::RenderWindow& window) const
{
	for (int i = 0; i < states.size(); i++)
		states[i]->drawTransitions(window);
	for (int i = 0; i < states.size(); i++)
		states[i]->draw(window);
}
void Animator::handleEvent(const sf::Event& event)
{
	//If we pressed delete and a state is selected, delete it
	if (event.type == Event::KeyPressed && event.key.code == Keyboard::Delete && selectedState != nullptr && !isAddingTransition)
	{
		for (int i = 0; i < states.size(); i++)
		{
			if (states[i] == selectedState)
			{
				states.erase(states.begin() + i);
				delete selectedState;
				setSelectedState(nullptr);
				break;
			}
		}
	}

	if (isAddingTransition) {
		if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left && !clickUsed)
		{
			int found = -1;
			for (int i = states.size() - 1; i >= 0; i--)
			{
				if (states[i]->getShape().getGlobalBounds().contains(mousePositionInWorld) && states[i] != selectedState)
				{
					found = i;
					break;
				}
			}
			if(found != -1)
				selectedState->addTransition(*states[found]);
			
			isAddingTransition = false;
			clickUsed = true;
		}
	}
	

	for (int i=states.size()-1; i>=0; i--)
	{
		states[i]->handleEvent(event);
	}
	for(int i=0; i<states.size(); i++)
	{
		states[i]->handleTransitionEvent(event);
	}
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

void Animator::saveAnimator(const string& path) const
{
	ofstream file(path, ios::binary);
	if (!file.is_open())
	{
		cout << "Failed to open file for writing" << endl;
		return;
	}
	int size = states.size();
	file.write((char*)&size, sizeof(int));
	for (int i = 0; i < size; i++)
	{
		int nameSize = states[i]->getName().size();
		file.write((char*)&nameSize, sizeof(int));
		file.write(states[i]->getName().c_str(), nameSize);
		int pathSize = states[i]->getPath().size();
		file.write((char*)&pathSize, sizeof(int));
		file.write(states[i]->getPath().c_str(), pathSize);
	}
	file.close();
}

Animator::~Animator()
{
	for (int i = 0; i < states.size(); i++)
	{
		delete states[i];
	}
}
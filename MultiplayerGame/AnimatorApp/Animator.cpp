#include "Animator.h"
#include "StateMenu.h"
#include <iostream>
#include <fstream>
#include "Transition.h"

using namespace sf;
using namespace std;

State* Animator::selectedState = nullptr;

Animator::Animator()
{
	defaultStateIndex = -1;
}
void Animator::addState(const string& name, const string& path)
{
	for (int i = 0; i < states.size(); i++)
	{
		if (states[i]->getPath() == path)
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
	if (this->defaultStateIndex != -1)
		states[this->defaultStateIndex]->setColor(Color::White);
	this->defaultStateIndex = defaultStateIndex;
	states[defaultStateIndex]->setColor(Color::Green);
}
void Animator::setDefaultStateIndex(State* state)
{
	if (this->defaultStateIndex != -1)
		states[this->defaultStateIndex]->setColor(Color::White);
	for (int i = 0; i < states.size(); i++)
	{
		if (states[i] == state)
		{
			defaultStateIndex = i;
			states[i]->setColor(Color::Green);
			return;
		}
	}
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
				if (i >= defaultStateIndex)
					defaultStateIndex--;
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
			if (found != -1)
				selectedState->addTransition(*states[found]);

			isAddingTransition = false;
			clickUsed = true;
		}
	}


	for (int i = states.size() - 1; i >= 0; i--)
	{
		states[i]->handleEvent(event);
	}
	for (int i = 0; i < states.size(); i++)
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
	if (Animator::selectedState != nullptr && Animator::selectedState != selectedState)
		Animator::selectedState->deselect();
	Animator::selectedState = selectedState;
	if (selectedState != nullptr)
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
	file.write((char*)&size, sizeof(int)); //write the amount of states
	for (int i = 0; i < size; i++)
	{
		/*int nameSize = states[i]->getName().size();
		file.write((char*)&nameSize, sizeof(int));
		file.write(states[i]->getName().c_str(), nameSize);
		int pathSize = states[i]->getPath().size();
		file.write((char*)&pathSize, sizeof(int));
		file.write(states[i]->getPath().c_str(), pathSize);*/
		states[i]->saveState(file); //save the state
	}

	cout << "States saved" << endl;

	//Save transitions
	for (int i = 0; i < size; i++)
	{
		states[i]->saveTransitions(file);
	}

	int defaultStateIndexTmp = defaultStateIndex;
	if (defaultStateIndex == -1 && size > 0) {
		defaultStateIndexTmp = 0;
	}
	file.write((char*)&defaultStateIndexTmp, sizeof(int));

	file.close();
}
void Animator::loadAnimator(const string& path)
{
	//Clear the current states
	for (int i = 0; i < states.size(); i++)
	{
		delete states[i];
	}
	states.clear();
	selectedState = nullptr;

	ifstream file(path, ios::binary);
	if (!file.is_open())
	{
		cout << "Failed to open file for reading" << endl;
		return;
	}
	int size;
	file.read((char*)&size, sizeof(int)); //read the amount of states
	for (int i = 0; i < size; i++)
	{
		addState("", "");
		states[states.size() - 1]->loadState(file); //load the state
	}

	cout << "States loaded" << endl;
	cout << "Loading transitions" << endl;

	//Load transitions
	for (int i = 0; i < size; i++)
	{
		int transitionsCount;
		file.read((char*)&transitionsCount, sizeof(int)); //read the amount of transitions
		for (int j = 0; j < transitionsCount; j++)
		{
			//Load the path of the arrival state
			size_t size;
			file.read((char*)&size, sizeof(size_t)); //read the size of the path
			char* buffer = new char[size + 1];
			file.read(buffer, size); //read the path of the arrival state
			buffer[size] = '\0';
			string path = buffer;
			delete[] buffer;

			//Find the arrival state
			State* arrivalState = nullptr;
			for (int k = 0; k < states.size(); k++)
			{
				if (states[k]->getPath() == path)
				{
					arrivalState = states[k];
					break;
				}
			}

			//Create the transition
			states[i]->addTransition(*arrivalState);
			Transition* transition = states[i]->getTransitions()[states[i]->getTransitions().size() - 1];

			transition->loadTransitionConditions(file); //load the transition conditions
		}
	}

	file.read((char*)&defaultStateIndex, sizeof(int));
	setDefaultStateIndex(defaultStateIndex);

	file.close();
}
Animator::~Animator()
{
	for (int i = 0; i < states.size(); i++)
	{
		delete states[i];
	}
}
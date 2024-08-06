#include "Animator.h"
#include <fstream>
#include "TextureManager.h"

using namespace std;
using namespace sf;

Animator::Animator(Shape& shape) : defaultStateIndex(0), currentState(nullptr), shape(shape) {}

int Animator::getDefaultStateIndex() const {
	return defaultStateIndex;
}

const vector<State*>& Animator::getStates() const {
	return states;
}

State* Animator::getCurrentState() {
	return currentState;
}

void Animator::checkTransitions() {
	if (currentState) {
		for (int i = 0; i < currentState->getTransitions().size(); i++)
		{
			Transition* transition = currentState->getTransitions()[i];
			bool canTransition = true;
			for (int j = 0; j < transition->getConditionsCount(); j++)
			{
				string name = transition->getConditionName(j);
				int op = transition->getConditionOperator(j); // 0 = ==, 1 = !=, 2 = <, 3 = >, 4 = <=, 5 = >=
				const FloatingBool& value = transition->getConditionValue(j);
				if (variables.find(name) == variables.end())
				{
					canTransition = false;
					break;
				}
				if (op == 0)
				{
					if (variables[name].isFloat)
					{
						if (variables[name].floatValue != value.floatValue)
						{
							canTransition = false;
							break;
						}
					}
					else
					{
						if (variables[name].boolValue != value.boolValue)
						{
							canTransition = false;
							break;
						}
					}
				}
				else if (op == 1)
				{
					if (variables[name].isFloat)
					{
						if (variables[name].floatValue == value.floatValue)
						{
							canTransition = false;
							break;
						}
					}
					else
					{
						if (variables[name].boolValue == value.boolValue)
						{
							canTransition = false;
							break;
						}
					}
				}
				else if (op == 2)
				{
					if (variables[name].isFloat)
					{
						if (variables[name].floatValue >= value.floatValue)
						{
							canTransition = false;
							break;
						}
					}
					else
					{
						if (variables[name].boolValue >= value.boolValue)
						{
							canTransition = false;
							break;
						}
					}
				}
				else if (op == 3)
				{
					if (variables[name].isFloat)
					{
						if (variables[name].floatValue <= value.floatValue)
						{
							canTransition = false;
							break;
						}
					}
					else
					{
						if (variables[name].boolValue <= value.boolValue)
						{
							canTransition = false;
							break;
						}
					}
				}
				else if (op == 4)
				{
					if (variables[name].isFloat)
					{
						if (variables[name].floatValue > value.floatValue)
						{
							canTransition = false;
							break;
						}
					}
					else
					{
						if (variables[name].boolValue > value.boolValue)
						{
							canTransition = false;
							break;
						}
					}
				}
				else if (op == 5)
				{
					if (variables[name].isFloat)
					{
						if (variables[name].floatValue < value.floatValue)
						{
							canTransition = false;
							break;
						}
					}
					else
					{
						if (variables[name].boolValue < value.boolValue)
						{
							canTransition = false;
							break;
						}
					}
				}
			}
			if (canTransition)
			{
				cout<<"Transitioning to "<<transition->getArrivalState().getPath()<<endl;
				currentState = & transition->getArrivalState();
				currentState->resetAnimation();
				string name = currentState->getPath();
				name = name.substr(0, name.size() - 5);
				shape.setTexture(&TextureManager::getInstance().getRef(name));
				break;
			}
		}
	}
	else
	{
		currentState = states[defaultStateIndex];
		currentState->resetAnimation();
		string name = currentState->getPath();
		name = name.substr(0, name.size() - 5);
		shape.setTexture(&TextureManager::getInstance().getRef(name));
	}
}

void Animator::setVariable(const string& name, const FloatingBool& value) {
	variables[name] = value;
	checkTransitions();
}

void Animator::setFloat(const string& name, float value) {
	variables[name] = FloatingBool(value);
	checkTransitions();
}

void Animator::setBool(const string& name, bool value) {
	variables[name] = FloatingBool(value);
	checkTransitions();
}

void Animator::update(float deltaTime) {
	if (currentState) {
		currentState->update(deltaTime);
	}
}

const IntRect& Animator::getFrame() const {
	if (currentState) {
		//cout<<"Getting frame from "<<currentState->getPath()<<endl;
		return currentState->getFrame();
	}
	else {
		return IntRect();
	}
}

void Animator::loadAnimator(const string& path) {
	//Clear the current states
	for (int i = 0; i < states.size(); i++)
	{
		delete states[i];
	}
	states.clear();
	currentState = nullptr;

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
		states.push_back(new State(""));
		states[states.size() - 1]->loadState(file); //load the state
	}

	cout << "States loaded: " << size << endl;
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

	cout<<"Default state index: "<<defaultStateIndex<<endl;

	file.close();
}

Animator::~Animator() {
	for (int i = 0; i < states.size(); i++)
	{
		delete states[i];
	}
}
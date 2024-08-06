#include "State.h"
#include <fstream>

using namespace std;
using namespace sf;

State::State(const string& path) : path(path) {}

const vector<Transition*>& State::getTransitions() const {
	return transitions;
}

void State::update(float deltaTime) {
	animation.update(deltaTime);
}

void State::loadState(ifstream& file) {
	//file is binary
	size_t size;
	file.read((char*)&size, sizeof(size_t)); //read the size of the name
	char* name = new char[size + 1];
	file.read(name, size); //read name
	name[size] = '\0';
	delete[] name;
	file.read((char*)&size, sizeof(size_t)); //read the size of the path
	char* path = new char[size + 1];
	file.read(path, size); //read path
	path[size] = '\0';
	this->path = path;
	cout<<"Loading animation from "<<path<<endl;
	animation.loadAnimation(path);
	cout<<"Animation loaded\n";
	delete[] path;
	float x, y;
	file.read((char*)&x, sizeof(float)); //read x position
	file.read((char*)&y, sizeof(float)); //read y position
}

const IntRect& State::getFrame() const {
	return animation.getCurrentFrame();
}

void State::resetAnimation() {
	animation.resetTime();
}

const string& State::getPath() const {
	return path;
}

void State::addTransition(State& arrivalState) {
	for (int i = 0; i < transitions.size(); i++)
		if (&transitions[i]->getArrivalState() == &arrivalState)
			return;
	transitions.push_back(new Transition(*this, arrivalState));
	arrivalState.incomingTransitions.push_back(transitions[transitions.size() - 1]);
}

State::~State() {
	for (int i = 0; i < transitions.size(); i++)
	{
		delete transitions[i];
	}
	for (int i = 0; i < incomingTransitions.size(); i++)
	{
		delete incomingTransitions[i];
	}
}
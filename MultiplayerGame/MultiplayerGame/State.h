#pragma once

#include "Transition.h"
#include "Animation.h"

class State
{
private:
	std::vector<Transition*> transitions;
	std::vector<Transition*> incomingTransitions;

	std::string path;
	Animation animation;
public:
	State(const std::string& path);
	const std::vector<Transition*>& getTransitions() const;
	void addTransition(State& arrivalState);

	const sf::IntRect& getFrame() const;
	const std::string& getPath() const;

	void resetAnimation();
	void loadState(std::ifstream& file);
	void update(float deltaTime);
	~State();
};
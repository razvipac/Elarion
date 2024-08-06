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
	void update(float deltaTime);
	void loadState(std::ifstream& file);
	const sf::IntRect& getFrame() const;
	void resetAnimation();
	const std::string& getPath() const;
	void addTransition(State& arrivalState);
	~State();
};
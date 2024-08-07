#pragma once

#include "State.h"
#include "Button.h"

extern bool clickUsed;


class Animator
{
private:
	std::vector<State*> states; //Resizing the vector means the selectedState pointer will be invalidated, causing a crash
	static State* selectedState;
	int defaultStateIndex;
public:
	Animator();
	void addState(const std::string& name, const std::string& path);
	const std::vector<State*>& getStates() const;
	int getDefaultStateIndex() const;
	void setDefaultStateIndex(int defaultStateIndex);
	void setDefaultStateIndex(State* state);

	static State* getSelectedState();
	static void setSelectedState(State* selectedState);

	void saveAnimator(const std::string& path) const;
	void loadAnimator(const std::string& path);

	void draw(sf::RenderWindow& window) const;
	void handleEvent(const sf::Event& event);
	~Animator();
};
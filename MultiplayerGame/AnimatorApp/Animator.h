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
	void setDefaultStateIndex(int defaultStateIndex);
	int getDefaultStateIndex() const;
	const std::vector<State*>& getStates() const;
	void draw(sf::RenderWindow& window) const;
	void handleEvent(const sf::Event& event);
	static State* getSelectedState();
	static void setSelectedState(State* selectedState);
	void saveAnimator(const std::string& path) const;
	~Animator();
};
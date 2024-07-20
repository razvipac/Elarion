#pragma once
#include "SFML/Graphics.hpp"
extern bool clickUsed;

extern sf::Vector2f mousePositionInWorld;
extern bool isAddingTransition;

class State;
class Transition
{
private:
	State& startingState;
	State& arrivalState;
	sf::Vector2f offset;
	sf::RectangleShape shape;
	bool mouseIntersectsLine() const;
public:
	Transition(State& startingState, State& arrivalState);
	const State& getArrivalState() const;
	void draw(sf::RenderWindow& window) const;
	void setPosition(const sf::Vector2f& position);
	void handleEvent(const sf::Event& event);
	void updatePositionAndRotation();
	~Transition();
};
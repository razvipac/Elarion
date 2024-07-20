#pragma once
#include "SFML/Graphics.hpp"
extern bool clickUsed;

extern sf::Vector2f mousePositionInWorld;

class State;
class Transition
{
private:
	const State& arrivalState;
public:
	Transition(const State& arrivalState);
	const State& getArrivalState() const;
};
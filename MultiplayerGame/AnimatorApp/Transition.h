#pragma once
extern bool clickUsed;

class State;
class Transition
{
private:
	const State& arrivalState;
public:
	Transition(const State& arrivalState);
	const State& getArrivalState() const;
};
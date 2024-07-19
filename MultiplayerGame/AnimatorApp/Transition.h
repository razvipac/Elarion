#pragma once
class State;
class Transition
{
private:
	const State& arrivalState;
public:
	Transition(const State& arrivalState);
	const State& getArrivalState() const;
};
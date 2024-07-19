#include "Transition.h"
#include "State.h"
Transition::Transition(const State& arrivalState) : arrivalState(arrivalState) {}
const State& Transition::getArrivalState() const
{
	return arrivalState;
}

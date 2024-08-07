#include "Transition.h"
#include <fstream>

using namespace std;

Transition::Transition(State& departureState, State& arrivalState) : startingState(departureState), arrivalState(arrivalState) {}
State& Transition::getArrivalState() const {
	return arrivalState;
}
int Transition::getConditionsCount() const {
	return names.size();
}
const string& Transition::getConditionName(int index) const {
	return names[index];
}
int Transition::getConditionOperator(int index) const {
	return operators[index];
}
const FloatingBool& Transition::getConditionValue(int index) const {
	return values[index];
}
void Transition::loadTransitionConditions(ifstream& file) {
	//Load the amount of conditions
	int conditionsCount;
	file.read((char*)&conditionsCount, sizeof(int));

	//Load the conditions
	for (int i = 0; i < conditionsCount; i++)
	{
		//Load the size of the name
		size_t size;
		file.read((char*)&size, sizeof(size_t)); //read the size of the name
		char* buffer = new char[size + 1];
		file.read(buffer, size); //read the name
		buffer[size] = '\0';
		string name = buffer;
		delete[] buffer;

		//Load the operator
		int op;
		file.read((char*)&op, sizeof(int)); //read the operator

		//Load the type of the value
		bool isFloat;
		file.read((char*)&isFloat, sizeof(bool)); //read the type of the value

		//Load the value
		if (isFloat)
		{
			float value;
			file.read((char*)&value, sizeof(float)); //read the float value
			names.push_back(name);
			operators.push_back(op);
			values.push_back(FloatingBool(value));
		}
		else
		{
			bool value;
			file.read((char*)&value, sizeof(bool)); //read the bool value
			names.push_back(name);
			operators.push_back(op);
			values.push_back(FloatingBool(value));
		}
	}
}
Transition::~Transition() {}
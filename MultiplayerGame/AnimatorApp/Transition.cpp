#include "Transition.h"
#include "TransitionMenu.h"
#include "State.h"
#include <iostream>

using namespace std;

Transition::Transition(State& startingState, State& arrivalState) : startingState(startingState), arrivalState(arrivalState)
{
	updatePositionAndRotation();
	shape.setFillColor(sf::Color::Black);
	shape.setOrigin(0, shape.getSize().y / 2);
	offset = sf::Vector2f(0, 0);
	arrow.setPointCount(3);
	arrow.setPoint(0, sf::Vector2f(0, 0));
	arrow.setPoint(1, sf::Vector2f(20, 10));
	arrow.setPoint(2, sf::Vector2f(20, -10));
	arrow.setFillColor(sf::Color::Black);
	arrow.setOrigin(10, 0);
	arrow.setOutlineColor(sf::Color::White);
}
const State& Transition::getArrivalState() const
{
	return arrivalState;
}
void Transition::draw(sf::RenderWindow& window) const
{
	window.draw(shape);
	window.draw(arrow);
}
void Transition::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left && !clickUsed)
	{
		if (arrow.getGlobalBounds().contains(mousePositionInWorld))
		{
			cout << "Transition clicked" << endl;
			TransitionMenu::getInstance().setCurrentTransition(this);
			clickUsed = true;

		}
	}
}
void Transition::setOutline(bool outline)
{
	if (outline)
		arrow.setOutlineThickness(2);
	else
		arrow.setOutlineThickness(0);
}
void Transition::updatePositionAndRotation()
{
	// Assuming offset() is a method that returns a sf::Vector2f representing the offset
	sf::Vector2f startingPositionOffset = startingState.getPosition() - offset;
	sf::Vector2f arrivalPositionOffset = arrivalState.getPosition() + offset;

	shape.setPosition(startingPositionOffset);
	float distance = sqrt(pow(arrivalPositionOffset.x - startingPositionOffset.x, 2) + pow(arrivalPositionOffset.y - startingPositionOffset.y, 2));
	shape.setSize(sf::Vector2f(distance, 5));
	shape.setRotation(atan2(arrivalPositionOffset.y - startingPositionOffset.y, arrivalPositionOffset.x - startingPositionOffset.x) * 180 / 3.14159265);

	// Calculate the arrow's position (30% of the way along the line)
	sf::Vector2f arrowPosition = startingPositionOffset + (arrivalPositionOffset - startingPositionOffset) * 0.6f;
	arrow.setPosition(arrowPosition);

	// Calculate the arrow's rotation
	float angle = atan2(arrivalPositionOffset.y - startingPositionOffset.y, arrivalPositionOffset.x - startingPositionOffset.x);
	arrow.setRotation(angle * 180 / 3.14159265 + 180);
}
bool Transition::mouseIntersectsLine() const //Method is currently not used anymore
{
	sf::Vector2f startingPositionOffset = startingState.getPosition() - offset;
	sf::Vector2f arrivalPositionOffset = arrivalState.getPosition() + offset;

	float distance = sqrt(pow(arrivalPositionOffset.x - startingPositionOffset.x, 2) + pow(arrivalPositionOffset.y - startingPositionOffset.y, 2));
	float angle = atan2(arrivalPositionOffset.y - startingPositionOffset.y, arrivalPositionOffset.x - startingPositionOffset.x);
	float distanceFromStart = sqrt(pow(mousePositionInWorld.x - startingPositionOffset.x, 2) + pow(mousePositionInWorld.y - startingPositionOffset.y, 2));
	float distanceFromEnd = sqrt(pow(mousePositionInWorld.x - arrivalPositionOffset.x, 2) + pow(mousePositionInWorld.y - arrivalPositionOffset.y, 2));
	float distanceFromLine = abs(sin(angle) * (mousePositionInWorld.x - startingPositionOffset.x) - cos(angle) * (mousePositionInWorld.y - startingPositionOffset.y));

	return distanceFromLine < 2.5 && distanceFromStart + distanceFromEnd - distance < 2.5;
}
void Transition::addCondition(const std::string& name, int operatorIndex, const FloatingBool& value)
{
	names.push_back(name);
	operators.push_back(operatorIndex);
	values.push_back(value);
}
void Transition::modifyCondition(int index, const std::string& name, int operatorIndex, const FloatingBool& value)
{
	names[index] = name;
	operators[index] = operatorIndex;
	values[index] = value;
}
void Transition::removeCondition(int index)
{
	names.erase(names.begin() + index);
	operators.erase(operators.begin() + index);
	values.erase(values.begin() + index);
}
int Transition::getConditionsCount() const
{
	return names.size();
}
const string& Transition::getConditionName(int index) const
{
	return names[index];
}
int Transition::getConditionOperator(int index) const
{
	return operators[index];
}
const FloatingBool& Transition::getConditionValue(int index) const
{
	return values[index];
}
void Transition::saveTransition(std::ofstream& file) const
{
	//Save the path of the arrival state
	size_t size = arrivalState.getPath().size();
	file.write((char*)&size, sizeof(size_t)); //write the size of the path
	file.write(arrivalState.getPath().c_str(), size); //write the path of the arrival state

	//Save the amount of conditions
	int conditionsCount = names.size();
	file.write((char*)&conditionsCount, sizeof(int)); //write the amount of conditions

	//Save the conditions
	for (int i = 0; i < conditionsCount; i++)
	{
		//Save the size of the name
		size = names[i].size();
		file.write((char*)&size, sizeof(size_t)); //write the size of the name
		//Save the name
		file.write(names[i].c_str(), size); //write the name

		//Save the operator
		file.write((char*)&operators[i], sizeof(int)); //write the operator

		//Save the type of the value
		file.write((char*)&values[i].isFloat, sizeof(bool)); //write the type of the value

		//Save the value
		if (values[i].isFloat)
			file.write((char*)&values[i].floatValue, sizeof(float)); //write the float value
		else
			file.write((char*)&values[i].boolValue, sizeof(bool)); //write the bool value
	}
}
void Transition::loadTransitionConditions(std::ifstream& file)
{
	//Load the amount of conditions
	int conditionsCount;
	file.read((char*)&conditionsCount, sizeof(int)); //read the amount of conditions

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
			addCondition(name, op, FloatingBool(value));
		}
		else
		{
			bool value;
			file.read((char*)&value, sizeof(bool)); //read the bool value
			addCondition(name, op, FloatingBool(value));
		}
	}
}
Transition::~Transition()
{
	startingState.eraseTransition(this);
	arrivalState.eraseTransition(this);
	if (TransitionMenu::getCurrentTransition() == this)
		TransitionMenu::setCurrentTransition(nullptr);
}
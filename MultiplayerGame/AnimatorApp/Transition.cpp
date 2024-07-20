#include "Transition.h"
#include "State.h"
#include <iostream>

using namespace std;

Transition::Transition(State& startingState, State& arrivalState) : startingState(startingState), arrivalState(arrivalState)
{
	shape.setOutlineColor(sf::Color::Red);
	shape.setOutlineThickness(2);
	updatePositionAndRotation();
	shape.setFillColor(sf::Color::Black);
	shape.setOrigin(0, shape.getSize().y / 2);
	offset = sf::Vector2f(20, 20);
}
const State& Transition::getArrivalState() const
{
	return arrivalState;
}
void Transition::draw(sf::RenderWindow& window) const
{
	window.draw(shape);
}
void Transition::setPosition(const sf::Vector2f& position)
{
	shape.setPosition(position);
}
void Transition::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left && !clickUsed)
	{
		if (mouseIntersectsLine())
		{
			cout << "Transition clicked" << endl;
			clickUsed = true;
		}
	}
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
}


bool Transition::mouseIntersectsLine() const
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


Transition::~Transition()
{
	startingState.eraseTransition(this);
	arrivalState.eraseTransition(this);
}
#include "State.h"
#include "Animator.h"
using namespace std;
State::State() {
	name = "New State";
	path = "";
	shape.setSize(sf::Vector2f(100, 50));
	setPosition(sf::Vector2f(200, 200));
	shape.setFillColor(sf::Color::White);
	shape.setOutlineColor(sf::Color::Black);
	shape.setOutlineThickness(2);
	text.setFont(font);
	text.setString(name);
	text.setCharacterSize(20);
	text.setFillColor(sf::Color::Black);
	isSelected = false;
}

void State::addTransition(const State& arrivalState)
{
	transitions.push_back(Transition(arrivalState));
}
const vector<Transition>& State::getTransitions() const
{
	return transitions;
}

void State::setName(const string& name)
{
	this->name = name;
	text.setString(name);
}
const string& State::getName() const
{
	return name;
}

void State::setPath(const string& path)
{
	this->path = path;
}
const string& State::getPath() const
{
	return path;
}

void State::draw(sf::RenderWindow& window) const
{
	window.draw(shape);
	window.draw(text);
}

void State::setPosition(const sf::Vector2f& position)
{
	shape.setPosition(position);
	text.setPosition(position);
}

void State::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
	{
		sf::Vector2f mousePosition = sf::Vector2f(event.mouseButton.x, event.mouseButton.y);
		if (shape.getGlobalBounds().contains(mousePosition) && !clickUsed)
		{
			shape.setOutlineColor(sf::Color::Blue);
			clickUsed = true;
			isDragging = true;
			Animator::setSelectedState(this);
			isSelected = true;
			lastMousePosition = mousePosition;
		}

	}
	else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
	{
		isDragging = false;
	}
	else if (event.type == sf::Event::MouseMoved && isDragging)
	{
		sf::Vector2f mousePosition = sf::Vector2f(event.mouseMove.x, event.mouseMove.y);
		sf::Vector2f offset = mousePosition - lastMousePosition;
		shape.setPosition(shape.getPosition() + offset);
		text.setPosition(text.getPosition() + offset);
		lastMousePosition = mousePosition;
	}
}

bool State::getIsSelected() const
{
	return isSelected;
}

void State::update()
{
}

void State::deselect()
{
	isSelected = false;
	shape.setOutlineColor(sf::Color::Black);

}
#include "State.h"
using namespace std;
State::State() {
	name = "New State";
	path = "";
	shape.setSize(sf::Vector2f(100, 50));
	shape.setFillColor(sf::Color::White);
	shape.setOutlineColor(sf::Color::Black);
	shape.setOutlineThickness(1);
	font.loadFromFile("Resources/Roboto-Black.ttf");
	text.setFont(font);
	text.setString(name);
	text.setCharacterSize(20);
	text.setFillColor(sf::Color::Black);
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
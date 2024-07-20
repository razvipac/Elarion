#include "State.h"
#include "Animator.h"
using namespace std;
State::State(const string& name, const string& path) {
	this->name = name;
	this->path = path;
	shape.setSize(sf::Vector2f(150, 50));
	setPosition(sf::Vector2f(200, 200));
	shape.setFillColor(sf::Color::White);
	shape.setOutlineColor(sf::Color::Black);
	shape.setOutlineThickness(2);
	text.setFont(font);
	text.setString(name);
	text.setCharacterSize(15);
	text.setPosition(shape.getPosition().x + 10, shape.getPosition().y + 10);
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
	text.setPosition(position.x + 10, position.y + 10);
}

void State::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left && !clickUsed)
	{
		if (shape.getGlobalBounds().contains(mousePositionInWorld))
		{
			shape.setOutlineColor(sf::Color::Blue);
			clickUsed = true;
			isDragging = true;
			Animator::setSelectedState(this);
			isSelected = true;
			lastMousePosition = mousePositionInWorld;
		}
		else
			Animator::setSelectedState(nullptr);

	}
	else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
	{
		isDragging = false;
	}
	else if (event.type == sf::Event::MouseMoved && isDragging)
	{
		sf::Vector2f offset = mousePositionInWorld - lastMousePosition;
		shape.setPosition(shape.getPosition() + offset);
		text.setPosition(text.getPosition() + offset);
		lastMousePosition = mousePositionInWorld;
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

void State::saveState(std::ofstream& file) const
{
	//file is binary
	file.write((char*)name.size(), sizeof(size_t));
	file.write(name.c_str(), name.size());
	file.write((char*)path.size(), sizeof(size_t));
	file.write(path.c_str(), path.size());
	int size = transitions.size();
	file.write((char*)&size, sizeof(int));
	for (int i = 0; i < size; i++)
	{
		int nameSize = transitions[i].getArrivalState().getName().size();
		file.write((char*)&nameSize, sizeof(int));
		file.write(transitions[i].getArrivalState().getName().c_str(), nameSize);
	}
}
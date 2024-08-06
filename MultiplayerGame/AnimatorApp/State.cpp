#include "State.h"
#include "Animator.h"
using namespace std;
State::State(const string& name, const string& path) {
	this->name = name;
	this->path = path;
	shape.setSize(sf::Vector2f(150, 50));
	shape.setOrigin(75, 25); //center the origin (for rotation)
	setPosition(sf::Vector2f(200, 200));
	shape.setFillColor(sf::Color::White);
	shape.setOutlineColor(sf::Color::Black);
	shape.setOutlineThickness(2);
	text.setFont(font);
	text.setString(name);
	text.setCharacterSize(15);
	text.setOrigin(text.getLocalBounds().width / 2, text.getLocalBounds().height / 2);
	text.setFillColor(sf::Color::Black);
	isSelected = false;
}

void State::addTransition(State& arrivalState)
{
	for(int i = 0; i < transitions.size(); i++)
		if (&transitions[i]->getArrivalState() == &arrivalState)
			return;
	transitions.push_back(new Transition(*this, arrivalState));
	arrivalState.incomingTransitions.push_back(transitions[transitions.size() - 1]);
}
const vector<Transition*> State::getTransitions() const
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

void State::drawTransitions(sf::RenderWindow& window) const
{
	for (int i = 0; i < transitions.size(); i++)
		transitions[i]->draw(window);
}

void State::setPosition(const sf::Vector2f& position)
{
	shape.setPosition(position);
	text.setPosition(position.x, position.y);
}

const sf::Vector2f& State::getPosition() const
{
	return shape.getPosition();
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
		//shape.setPosition(shape.getPosition() + offset);
		//text.setPosition(text.getPosition() + offset);
		setPosition(shape.getPosition() + offset);
		lastMousePosition = mousePositionInWorld;
		for (int i = 0; i < transitions.size(); i++)
			transitions[i]->updatePositionAndRotation();
		for (int i = 0; i < incomingTransitions.size(); i++)
			incomingTransitions[i]->updatePositionAndRotation();
	}
}

void State::handleTransitionEvent(const sf::Event& event)
{
	for (int i = 0; i < transitions.size(); i++)
		transitions[i]->handleEvent(event);
}

const sf::RectangleShape& State::getShape() const
{
	return shape;
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
	size_t size = name.size();
	file.write((char*)&size, sizeof(size_t)); //write the size of the name
	file.write(name.c_str(), name.size()); //write name
	size = path.size();
	file.write((char*)&size, sizeof(size_t)); //write the size of the path
	file.write(path.c_str(), path.size()); //write path
	sf::Vector2f position = shape.getPosition();
	file.write((char*)&position.x, sizeof(float)); //write x position
	file.write((char*)&position.y, sizeof(float)); //write y position
	//size = transitions.size();
	//file.write((char*)&size, sizeof(int)); //write the amount of transitions
	//for (int i = 0; i < size; i++)
	//{
	//	/*int nameSize = transitions[i]->getArrivalState().getName().size();
	//	file.write((char*)&nameSize, sizeof(int));
	//	file.write(transitions[i]->getArrivalState().getName().c_str(), nameSize);*/
	//	transitions[i]->saveTransition(file); //save transition
	//}
}

void State::saveTransitions(std::ofstream& file) const
{
	//file is binary
	int size = transitions.size();
	file.write((char*)&size, sizeof(int)); //write the amount of transitions
	for (int i = 0; i < size; i++)
	{
		transitions[i]->saveTransition(file); //save transition
	}
}

void State::loadState(std::ifstream& file)
{
	//file is binary
	size_t size;
	file.read((char*)&size, sizeof(size_t)); //read the size of the name
	cout<<"Size of name: "<<size<<endl;
	char* name = new char[size + 1];
	file.read(name, size); //read name
	name[size] = '\0';
	setName(name);
	delete[] name;
	cout<<"Name: "<<getName()<<endl;
	file.read((char*)&size, sizeof(size_t)); //read the size of the path
	cout<<"Size of path: "<<size<<endl;
	char* path = new char[size + 1];
	file.read(path, size); //read path
	path[size] = '\0';
	setPath(path);
	delete[] path;
	cout<<"Path: "<<getPath()<<endl;
	float x, y;
	file.read((char*)&x, sizeof(float)); //read x position
	file.read((char*)&y, sizeof(float)); //read y position
	setPosition(sf::Vector2f(x, y));
	text.setOrigin(text.getLocalBounds().width / 2, text.getLocalBounds().height / 2);
}

void State::eraseTransition(Transition* transition)
{
	for (int i = 0; i < transitions.size(); i++)
	{
		if (transitions[i] == transition)
		{
			transitions.erase(transitions.begin() + i);
			break;
		}
	}
	for (int i = 0; i < incomingTransitions.size(); i++)
	{
		if (incomingTransitions[i] == transition)
		{
			incomingTransitions.erase(incomingTransitions.begin() + i);
			break;
		}
	}
}

void State::setColor(const sf::Color& color)
{
	shape.setFillColor(color);
}

State::~State()
{
	// delete transitions
	for (int i = 0; i < transitions.size(); i++)
		delete transitions[i];
	for (int i = incomingTransitions.size() - 1; i >= 0; i--)
		delete incomingTransitions[i];
}
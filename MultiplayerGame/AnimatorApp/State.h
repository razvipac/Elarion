#pragma once
#include "Transition.h"
#include <vector>
#include <iostream>
#include "SFML/Graphics.hpp"
class State
{
private:
	std::vector<Transition> transitions;
	std::string name;
	std::string path;
	sf::RectangleShape shape;
	sf::Text text;
	sf::Font font;
public:
	State();
	void addTransition(const State& arrivalState);
	const std::vector<Transition>& getTransitions() const;
	void setName(const std::string& name);
	const std::string& getName() const;
	void setPath(const std::string& path);
	const std::string& getPath() const;
	void draw(sf::RenderWindow& window) const;
	void setPosition(const sf::Vector2f& position);
};
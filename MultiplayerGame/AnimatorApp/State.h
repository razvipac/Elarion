#pragma once

#include "Transition.h"
#include <vector>
#include <iostream>
#include "SFML/Graphics.hpp"
#include <fstream>

extern sf::Font font;
extern bool clickUsed;


class State
{
private:
	std::vector<Transition*> transitions;
	std::vector<Transition*> incomingTransitions;
	std::string name;
	std::string path;
	sf::RectangleShape shape;
	sf::Text text;
	bool isDragging;
	bool isSelected;
	sf::Vector2f lastMousePosition;
public:
	State(const std::string& name, const std::string& path);
	void addTransition(State& arrivalState);
	const std::vector<Transition*> getTransitions() const;
	void setName(const std::string& name);
	const std::string& getName() const;
	void setPath(const std::string& path);
	const std::string& getPath() const;
	void draw(sf::RenderWindow& window) const;
	void drawTransitions(sf::RenderWindow& window) const;
	const sf::RectangleShape& getShape() const;
	void setPosition(const sf::Vector2f& position);
	const sf::Vector2f& getPosition() const;
	void handleEvent(const sf::Event& event);
	void handleTransitionEvent(const sf::Event& event);
	void update();
	bool getIsSelected() const;
	void deselect();
	void saveState(std::ofstream& file) const;
	void saveTransitions(std::ofstream& file) const;
	void loadState(std::ifstream& file);
	void eraseTransition(Transition* transition);
	void setColor(const sf::Color& color);
	~State();
};
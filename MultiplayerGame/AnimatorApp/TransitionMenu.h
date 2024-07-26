#pragma once
#include "SFML/Graphics.hpp"
#include "Animator.h"
#include "ConditionUI.h"
#include <vector>

extern bool clickUsed;
extern sf::Vector2f mousePositionInUI;
extern sf::Font font;

class TransitionMenu
{
private:
	sf::RectangleShape shape;
	Animator* animator;
	Button addConditionButton;
	static Transition* currentTransition;
	std::vector<ConditionUI> conditionUIs;
	TransitionMenu(const sf::Vector2f& size, const sf::Vector2f& position);
public:
	TransitionMenu& operator=(const TransitionMenu&) = delete;
	TransitionMenu(const TransitionMenu&) = delete;
	static TransitionMenu& getInstance();
	void draw(sf::RenderWindow& window) const;
	void handleEvent(const sf::Event& event);
	void setAnimator(Animator* animator);
	static void setCurrentTransition(Transition* transition);
	static Transition* getCurrentTransition();
	void removeConditionUI(int index);
}
;
#include "TransitionMenu.h"
#include "Animator.h"
#include "Button.h"

using namespace sf;
using namespace std;

Transition* TransitionMenu::currentTransition = nullptr;

TransitionMenu::TransitionMenu(const Vector2f& size, const Vector2f& position) : addConditionButton(Vector2f(180, 50), Vector2f(position.x + 10, position.y + 10), "Add Condition", [this]() {})
{
	shape.setSize(size);
	shape.setPosition(position);
	shape.setFillColor(Color::White);
	shape.setOutlineColor(Color::Black);
	shape.setOutlineThickness(2);
	addConditionButton.setOnClick([this]()
		{
			if (currentTransition != nullptr)
			{
				currentTransition->addCondition("Variable Name", 0, FloatingBool(true));
				float y = 20;
				if (conditionUIs.size() > 0)
					y = conditionUIs[conditionUIs.size() - 1]->getPosition().y + 300;
				conditionUIs.push_back(new ConditionUI(Vector2f(1080, y), currentTransition->getConditionsCount() - 1));
			}
		});

	animator = nullptr;
}
TransitionMenu& TransitionMenu::getInstance()
{
	static TransitionMenu instance(Vector2f(200, 720), Vector2f(1080, 0));
	return instance;
}
void TransitionMenu::draw(RenderWindow& window) const
{
	window.draw(shape);
	for (int i = 0; i < conditionUIs.size(); i++)
	{
		conditionUIs[i]->draw(window);
	}
	addConditionButton.draw(window);
}
void TransitionMenu::handleEvent(const Event& event)
{
	//if we are scrolling, move the conditions (using setposition)
	if (event.type == Event::MouseWheelScrolled && shape.getGlobalBounds().contains(mousePositionInUI) && conditionUIs.size() > 1)
	{
		bool ok = true;
		if (event.mouseWheelScroll.delta > 0 && conditionUIs[0]->getPosition().y > 20)
			ok = false;
		if (event.mouseWheelScroll.delta < 0 && conditionUIs[conditionUIs.size() - 1]->getPosition().y < 300)
			ok = false;
		if (ok) {
			for (int i = 0; i < conditionUIs.size(); i++)
			{
				conditionUIs[i]->setPosition(Vector2f(1080, conditionUIs[i]->getPosition().y + event.mouseWheelScroll.delta * 10));
			}
		}
	}

	addConditionButton.handleEvent(event);
	for (int i = 0; i < conditionUIs.size(); i++)
	{
		conditionUIs[i]->handleEvent(event);
	}
}
void TransitionMenu::setAnimator(Animator* animator) {
	this->animator = animator;
}
void TransitionMenu::setCurrentTransition(Transition* transition) {
	getInstance().conditionUIs.clear();
	if (currentTransition != nullptr)
		currentTransition->setOutline(false);
	currentTransition = transition;
	if (currentTransition != nullptr) {
		for (int i = 0; i < currentTransition->getConditionsCount(); i++)
		{
			getInstance().conditionUIs.push_back(new ConditionUI(Vector2f(1080, 70 + i * 300), i));
			getInstance().conditionUIs.back()->setValues(currentTransition->getConditionName(i), currentTransition->getConditionOperator(i), currentTransition->getConditionValue(i).toStringLmao(), currentTransition->getConditionValue(i).isFloat);
		}
		currentTransition->setOutline(true);
	}
}
Transition* TransitionMenu::getCurrentTransition() {
	return currentTransition;
}
void TransitionMenu::removeConditionUI(int index) {
	delete conditionUIs[index];
	conditionUIs.erase(conditionUIs.begin() + index);
	for (int i = index; i < conditionUIs.size(); i++)
	{
		conditionUIs[i]->setPosition(Vector2f(1080, 70 + (i - 1) * 300));
		conditionUIs[i]->setIndex(i - 1);
	}
}
const RectangleShape& TransitionMenu::getShape() const {
	return shape;
}
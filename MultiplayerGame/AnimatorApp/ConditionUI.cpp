#include "ConditionUI.h"
#include "Animator.h"
#include "Button.h"
#include "TransitionMenu.h"

using namespace std;
using namespace sf;

ConditionUI::ConditionUI(const Vector2f& position, int index) : position(position), variableNameInputField(Vector2f(180, 25), Vector2f(position.x + 10, position.y + 120), "Variable Name"), operatorInputField(Vector2f(180, 25), Vector2f(position.x + 10, position.y + 170), "Operator"), valueInputField(Vector2f(180, 25), Vector2f(position.x + 10, position.y + 220), "Value"), typeInputField(Vector2f(180, 25), Vector2f(position.x + 10, position.y + 270), "Type"), removeButton(Vector2f(30, 30), Vector2f(position.x + 10, position.y + 320), "X", [index]() {
	if (TransitionMenu::getCurrentTransition() != nullptr)
	{
		TransitionMenu::getCurrentTransition()->removeCondition(index);
		TransitionMenu::getInstance().removeConditionUI(index);
	}})
{
	// position them such that they are below each other
	text.setFont(font);
	text.setString("Condition " + to_string(index + 1));
	text.setCharacterSize(15);
	text.setFillColor(Color::Black);
	text.setPosition(position.x + 10, position.y + 70);

	variableName.setFont(font);
	variableName.setString("Variable Name");
	variableName.setCharacterSize(15);
	variableName.setFillColor(Color::Black);
	variableName.setPosition(position.x + 10, position.y + 100);

	operatorText.setFont(font);
	operatorText.setString("Operator");
	operatorText.setCharacterSize(15);
	operatorText.setFillColor(Color::Black);
	operatorText.setPosition(position.x + 10, position.y + 150);

	valueText.setFont(font);
	valueText.setString("Value");
	valueText.setCharacterSize(15);
	valueText.setFillColor(Color::Black);
	valueText.setPosition(position.x + 10, position.y + 200);

	type.setFont(font);
	type.setString("Type");
	type.setCharacterSize(15);
	type.setFillColor(Color::Black);
	type.setPosition(position.x + 10, position.y + 250);

	variableNameInputField.setOnFinishEdit([this](const string& name) {
		nameString = name;
		updateCondition();
		});

	operatorInputField.setOnFinishEdit([this](const string& operatorr) {
		operatorString = operatorr;
		updateCondition();
		});

	valueInputField.setOnFinishEdit([this](const string& value) {
		valueString = value;
		updateCondition();
		});

	this->index = index;
}
int ConditionUI::getTypeFromOperator(const string& operatorString) const {
	if (operatorString == "!=")
		return 1;
	if (operatorString == "<")
		return 2;
	if (operatorString == ">")
		return 3;
	if (operatorString == "<=")
		return 4;
	if (operatorString == ">=")
		return 5;
	return 0;
}
string getOperatorFromType(int type) {
	if (type == 1)
		return "!=";
	if (type == 2)
		return "<";
	if (type == 3)
		return ">";
	if (type == 4)
		return "<=";
	if (type == 5)
		return ">=";
	return "==";
}
void ConditionUI::draw(RenderWindow& window) const
{
	window.draw(text);
	window.draw(variableName);
	window.draw(operatorText);
	window.draw(valueText);
	window.draw(type);
	variableNameInputField.draw(window);
	operatorInputField.draw(window);
	valueInputField.draw(window);
	typeInputField.draw(window);
	removeButton.draw(window);
}
void ConditionUI::handleEvent(const Event& event)
{
	variableNameInputField.handleEvent(event);
	operatorInputField.handleEvent(event);
	valueInputField.handleEvent(event);
	typeInputField.handleEvent(event);
	removeButton.handleEvent(event);
}
void ConditionUI::setPosition(const Vector2f& position)
{
	this->position = position;
	text.setPosition(position.x + 10, position.y + 70);
	variableName.setPosition(position.x + 10, position.y + 100);
	variableNameInputField.setPosition(Vector2f(position.x + 10, position.y + 120));
	operatorText.setPosition(position.x + 10, position.y + 150);
	operatorInputField.setPosition(Vector2f(position.x + 10, position.y + 170));
	valueText.setPosition(position.x + 10, position.y + 200);
	valueInputField.setPosition(Vector2f(position.x + 10, position.y + 220));
	type.setPosition(position.x + 10, position.y + 250);
	typeInputField.setPosition(Vector2f(position.x + 10, position.y + 270));
	removeButton.setPosition(Vector2f(position.x + 10, position.y + 320));
}
const Vector2f& ConditionUI::getPosition() const
{
	return position;
}
void ConditionUI::setIndex(int index)
{
	text.setString("Condition " + to_string(index + 1));
	removeButton.setOnClick([index]() {
		if (TransitionMenu::getCurrentTransition() != nullptr)
		{
			TransitionMenu::getCurrentTransition()->removeCondition(index);
			TransitionMenu::getInstance().removeConditionUI(index);
		}
		});
}
void ConditionUI::setValues(const string& variableName, int operatorIndex, const string& value, bool isFloat)
{
	variableNameInputField.setText(variableName);
	operatorInputField.setText(getOperatorFromType(operatorIndex));
	valueInputField.setText(value);
	if (isFloat)
		typeInputField.setText("float");
	else
		typeInputField.setText("bool");

	nameString = variableName;
	operatorString = getOperatorFromType(operatorIndex);
	valueString = value;
}
void ConditionUI::updateCondition() const
{
	if (TransitionMenu::getCurrentTransition() != nullptr)
	{
		int type = getTypeFromOperator(operatorString);
		try {
			float value = 0;
			value = stof(valueString);
			TransitionMenu::getCurrentTransition()->modifyCondition(index, nameString, type, FloatingBool(value));
		}
		catch (...) {
			bool value = false;
			if (valueString == "true")
				value = true;
			TransitionMenu::getCurrentTransition()->modifyCondition(index, nameString, type, FloatingBool(value));
		}
	}
}
ConditionUI::~ConditionUI()
{
	cout << "ConditionUI destroyed" << endl;
}
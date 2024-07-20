#include "StateMenu.h"
#include "FileSelection.h"

using namespace std;
using namespace sf;
//	InputField(const sf::Vector2f& size, const sf::Vector2f& position, const std::string& text, const std::function<void(const std::string&)>& onTextChange);

StateMenu::StateMenu(const Vector2f& size, const Vector2f& position) : /*inputField(Vector2f(120, 40), Vector2f(position.x + 10, position.y + 40), ""),*/ addStateButton(Vector2f(175, 50), Vector2f(position.x + 10, position.y + 600), "Add State", [this]() {}), addTransitionButton(Vector2f(175, 50), Vector2f(position.x + 10, position.y + 125), "Add Transition", [this]() {})
{
	shape.setSize(size);
	shape.setPosition(position);
	shape.setFillColor(Color::White);
	shape.setOutlineColor(Color::Black);
	shape.setOutlineThickness(1);
	this->text.setFont(font);
	this->text.setString("No state selected.");
	this->text.setCharacterSize(20);
	this->text.setFillColor(Color::Black);
	this->text.setPosition(position.x + 10, position.y + 10);
	/*inputField.setOnFinishEdit([this](const string& text) {
		if (Animator::getSelectedState() != nullptr)
			Animator::getSelectedState()->setName(text);
		});*/
	animator = nullptr;
}

StateMenu& StateMenu::getInstance()
{
	static StateMenu instance(Vector2f(200, 720), Vector2f(0, 0));
	return instance;
}

void StateMenu::draw(RenderWindow& window) const
{
	window.draw(shape);
	window.draw(text);
	//inputField.draw(window);
	addStateButton.draw(window);
	addTransitionButton.draw(window);
}

void StateMenu::handleEvent(const Event& event)
{
	//inputField.handleEvent(event);
	addStateButton.handleEvent(event);
	addTransitionButton.handleEvent(event);
}

void StateMenu::setText(const string& text)
{
	//inputField.setText(text);
	if(text.size())
		this->text.setString(text);
	else
		this->text.setString("No state selected.");
}

const string& StateMenu::getText() const
{
	return text.getString();
}

void StateMenu::setAnimator(Animator* animator) {
	this->animator = animator;
	addStateButton.setOnClick([animator]() {
		string path = OpenFileDialog();
		if (path == "")
			return;
		string nameWithExtension = path.substr(path.find_last_of("\\")); // Get the name of the file
		nameWithExtension = nameWithExtension.substr(1); // Remove the backslash
		string name = nameWithExtension.substr(0, nameWithExtension.find_last_of(".")); // Remove the extension

		animator->addState(name, path);
	});

	addTransitionButton.setOnClick([animator]() {
		if(animator->getSelectedState() == nullptr)
			return;
		isAddingTransition = true;
		cout << "Adding transition" << endl;
		//animator->addTransition();
	});
}
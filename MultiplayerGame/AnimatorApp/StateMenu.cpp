#include "StateMenu.h"
#include "FileSelection.h"
#include <filesystem>

using namespace std;
using namespace sf;

StateMenu::StateMenu(const Vector2f& size, const Vector2f& position) : /*inputField(Vector2f(120, 40), Vector2f(position.x + 10, position.y + 40), ""),*/ addStateButton(Vector2f(175, 50), Vector2f(position.x + 10, position.y + 600), "Add State", [this]() {}), addTransitionButton(Vector2f(175, 50), Vector2f(position.x + 10, position.y + 200), "Add Transition", [this]() {}), saveButton(Vector2f(175, 50), Vector2f(position.x + 10, position.y + 50), "Save", [this]() {}), loadButton(Vector2f(175, 50), Vector2f(position.x + 10, position.y + 125), "Load", [this]() {}), setDefaultButton(Vector2f(175, 50), Vector2f(position.x + 10, position.y + 275), "Set Default", [this]() {})
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

	animator = nullptr;
	saveButton.setOnClick([this]() {
		string path = SaveFileDialog();
		if (path == "")
			return;
		animator->saveAnimator(path);
		});
	loadButton.setOnClick([this]() {
		string path = OpenFileDialog(false);
		if (path == "")
			return;
		animator->loadAnimator(path);
		});

	setDefaultButton.setOnClick([this]() {
		if (animator->getSelectedState() == nullptr)
			return;
		animator->setDefaultStateIndex(animator->getSelectedState());
		});
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
	loadButton.draw(window);
	saveButton.draw(window);
	addStateButton.draw(window);
	addTransitionButton.draw(window);
	setDefaultButton.draw(window);
}
void StateMenu::handleEvent(const Event& event)
{
	setDefaultButton.handleEvent(event);
	addStateButton.handleEvent(event);
	addTransitionButton.handleEvent(event);
	saveButton.handleEvent(event);
	loadButton.handleEvent(event);
}
void StateMenu::setText(const string& text)
{
	if (text.size())
		this->text.setString(text);
	else
		this->text.setString("No state selected.");
}
string StateMenu::getText() const
{
	return text.getString();
}
void StateMenu::setAnimator(Animator* animator) {
	this->animator = animator;
	addStateButton.setOnClick([animator]() {
		string path = OpenFileDialog();
		if (path == "")
			return;

		// Convert absolute path to relative path
		filesystem::path absolutePath(path);
		filesystem::path relativePath = filesystem::relative(absolutePath);

		string nameWithExtension = relativePath.filename().string(); // Get the name of the file
		string name = nameWithExtension.substr(0, nameWithExtension.find_last_of(".")); // Remove the extension

		cout << "Adding state " << name << " with path " << relativePath.string() << endl;
		if (filesystem::exists(relativePath.string()) == false)
			cout << "File does not exist" << endl;
		else
			cout << "File exists" << endl;

		animator->addState(name, relativePath.string());
		});

	addTransitionButton.setOnClick([animator]() {
		if (animator->getSelectedState() == nullptr)
			return;
		isAddingTransition = true;
		cout << "Adding transition" << endl;
		});
}
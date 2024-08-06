#include "Animator.h"
#include "StateMenu.h"
#include "TransitionMenu.h"
#include <iostream>

using namespace std;
using namespace sf;

Font font;
bool clickUsed = false;
Vector2f mousePositionInWorld;
Vector2f mousePositionInUI;
bool isAddingTransition = false;

int main()
{
	if(!font.loadFromFile("Resources/Roboto-Black.ttf"))
	{
		cout << "Error loading font" << endl;
	}

	RenderWindow window(VideoMode(1280, 720), "[Multiplayer Java Game] Animator Tool");
	Animator animator;
	StateMenu& stateMenu = StateMenu::getInstance();
	TransitionMenu& transitionMenu = TransitionMenu::getInstance();
	stateMenu.setAnimator(&animator);
	transitionMenu.setAnimator(&animator);

	View worldView = window.getDefaultView();
	View uiView = window.getDefaultView();

	Vector2f lastMousePos;

	Color backgroundColor(89, 89, 89);

	while (window.isOpen())
	{
		mousePositionInWorld = window.mapPixelToCoords(Mouse::getPosition(window), worldView);
		mousePositionInUI = window.mapPixelToCoords(Mouse::getPosition(window), uiView);
		clickUsed = false;
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				window.close();
			}
			else if (event.type == Event::MouseWheelScrolled && !transitionMenu.getShape().getGlobalBounds().contains(mousePositionInUI)) { // Controls zooming in and out of the 2D world view
				if (event.mouseWheelScroll.delta > 0) {
					worldView.zoom(0.9f);
				}
				else {
					worldView.zoom(1.1f);
				}
			}
			stateMenu.handleEvent(event);
			transitionMenu.handleEvent(event);
			animator.handleEvent(event);
		}

		//Controls panning the camera in the 2D world view
		if (Mouse::isButtonPressed(Mouse::Right)) {
			lastMousePos = window.mapPixelToCoords(Vector2i(lastMousePos.x, lastMousePos.y), worldView);
			worldView.move(lastMousePos - mousePositionInWorld);
		}
		lastMousePos = Vector2f(Mouse::getPosition(window).x, Mouse::getPosition(window).y);




		window.clear(backgroundColor);
		window.setView(worldView);
		animator.draw(window);
		window.setView(uiView);
		stateMenu.draw(window);
		transitionMenu.draw(window);
		window.display();
	}

}
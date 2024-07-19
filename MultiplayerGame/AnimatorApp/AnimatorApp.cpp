// AnimatorApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "Animator.h"
#include "StateMenu.h"
#include <iostream>

using namespace std;
using namespace sf;

Font font;
bool clickUsed = false;

int main()
{
	if(!font.loadFromFile("Resources/Roboto-Black.ttf"))
	{
		cout << "Error loading font" << endl;
	}

	RenderWindow window(VideoMode(800, 600), "Window");
	Animator animator;
	StateMenu& stateMenu = StateMenu::getInstance();

	while (window.isOpen())
	{
		clickUsed = false;
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				window.close();
			}
			stateMenu.handleEvent(event);
			animator.handleEvent(event);
		}
		window.clear();
		animator.draw(window);
		stateMenu.draw(window);
		window.display();
	}

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

#include <iostream>
#include <SFML/Graphics.hpp>
#include "MapEditor.h"
#include "TilesEditor.h"

using namespace std;
using namespace sf;

Font font;
Vector2f mousePosInWorld;
Vector2f mousePosInUI;
int indexOfSelectedTile = 65;

int main()
{
    if (!font.loadFromFile("Resources/Roboto-Black.ttf"))
	{
		cout << "Error loading font" << endl;
	}

	MapEditor mapEditor;
	TilesEditor tilesEditor;

	RenderWindow window(VideoMode(1280, 720), "[Multiplayer Java Game] Map Editor Tool");

	View worldView = window.getDefaultView();
	View uiView = window.getDefaultView();

	Vector2f lastMousePos;

	RectangleShape square = RectangleShape(Vector2f(16, 16));
	square.setFillColor(Color::Black);
	square.setOutlineColor(Color::White);
	square.setOutlineThickness(1);


	Clock clock;
	float deltaTime;

	while (window.isOpen())
	{
		mousePosInWorld = window.mapPixelToCoords(Mouse::getPosition(window), worldView);
		mousePosInUI = window.mapPixelToCoords(Mouse::getPosition(window), uiView);
		deltaTime = clock.restart().asSeconds();
		Event event;

		while (window.pollEvent(event))
		{
			mapEditor.handleEvent(event);
			tilesEditor.handleEvent(event);
			if (event.type == Event::Closed)
			{
				window.close();
			}
			if (event.type == Event::MouseWheelScrolled)
			{
				if (event.mouseWheelScroll.wheel == Mouse::VerticalWheel)
				{
					if (event.mouseWheelScroll.delta > 0)
					{
						worldView.zoom(0.9f);
					}
					else
					{
						worldView.zoom(1.1f);
					}
				}
			}
		}

		if (Mouse::isButtonPressed(Mouse::Right)) {
			lastMousePos = window.mapPixelToCoords(Vector2i(lastMousePos.x, lastMousePos.y), worldView);
			worldView.move(lastMousePos - mousePosInWorld);
		}
		lastMousePos = Vector2f(Mouse::getPosition(window).x, Mouse::getPosition(window).y);

		mapEditor.update(deltaTime);

		window.clear(Color(89, 89, 89));

		window.setView(worldView);
		mapEditor.draw(window);
		window.setView(uiView);
		tilesEditor.draw(window);
		window.display();
	}
}

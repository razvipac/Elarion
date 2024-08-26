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
bool blockClick;
bool blockScroll;
int selectedLayer;

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

	Text layerText;
	layerText.setFont(font);
	layerText.setCharacterSize(24);
	layerText.setFillColor(Color::White);
	layerText.setPosition(350, 10);
	layerText.setString("Layer: 0");
	layerText.setOutlineColor(Color::Black);
	layerText.setOutlineThickness(1);

	while (window.isOpen())
	{
		blockClick = blockScroll = false;
		mousePosInWorld = window.mapPixelToCoords(Mouse::getPosition(window), worldView);
		mousePosInUI = window.mapPixelToCoords(Mouse::getPosition(window), uiView);
		deltaTime = clock.restart().asSeconds();
		Event event;

		while (window.pollEvent(event))
		{
			tilesEditor.handleEvent(event);
			mapEditor.handleEvent(event);
			if (event.type == Event::Closed)
			{
				window.close();
			}
			if (event.type == Event::MouseWheelScrolled && !blockScroll)
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
			// if L is pressed then change the layer to 0
			if (event.type == Event::KeyReleased && event.key.code == Keyboard::L)
			{
				selectedLayer = 1 - selectedLayer;
				layerText.setString("Layer: " + to_string(selectedLayer));
			}
		}

		if (Mouse::isButtonPressed(Mouse::Right)) {
			lastMousePos = window.mapPixelToCoords(Vector2i(lastMousePos.x, lastMousePos.y), worldView);
			worldView.move(lastMousePos - mousePosInWorld);
		}
		lastMousePos = Vector2f(Mouse::getPosition(window).x, Mouse::getPosition(window).y);

		tilesEditor.update(deltaTime);
		mapEditor.update(deltaTime);

		window.clear(Color(89, 89, 89));

		window.setView(worldView);
		mapEditor.draw(window);
		window.setView(uiView);
		tilesEditor.draw(window);
		window.draw(layerText);
		window.display();
	}
}

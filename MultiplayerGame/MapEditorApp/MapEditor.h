#pragma once
#include "TileMap.h"
#include <SFML/Graphics.hpp>

class MapEditor
{
private:
	TileMap tileMap;
	bool pressedLastFrame = false;
	sf::Vector2f lastMousePos;
public:
	MapEditor();
	void draw(sf::RenderTarget& target) const;
	void update(float deltaTime);
	void handleEvent(sf::Event event);
	~MapEditor();
};
#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>

class TilesEditor
{
private:
	sf::Texture texture;
	sf::RectangleShape background;
	std::vector<sf::RectangleShape> tiles;
	int totalAmountOfTiles;
	int tilesPerRow;
	int selectedTileIndex;
	std::vector<int> tileToTextureIndex;
public:
	TilesEditor();
	void draw(sf::RenderTarget& target) const;
	void update(float deltaTime);
	void handleEvent(sf::Event event);
	~TilesEditor();
};
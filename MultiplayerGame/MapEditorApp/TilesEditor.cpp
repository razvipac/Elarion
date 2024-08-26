#include "TilesEditor.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <fstream>

extern sf::Vector2f mousePosInUI;
extern int indexOfSelectedTile;
extern bool blockClick;
extern bool blockScroll;

using namespace std;

TilesEditor::TilesEditor()
{
	texture.loadFromFile("Resources/Tileset/spr_tileset_sunnysideworld_16px.png");

	background.setSize(sf::Vector2f(340, 720));
	background.setFillColor(sf::Color::White);
	background.setPosition(0, 0);

	tilesPerRow = 10;
	int tileSize = 16;
	selectedTileIndex = 10;

	int yLimit = texture.getSize().y / tileSize;
	// look through the texture and create a rectangle shape for each tile which is not completely empty
	/*for (int y = 0; y < yLimit; y++)
	{
		for (int x = 0; x < texture.getSize().x / tileSize; x++)
		{
			sf::IntRect rect(x * tileSize, y * tileSize, tileSize, tileSize);
			sf::Image img = texture.copyToImage();
			bool empty = true;
			for (int i = 0; i < tileSize; i++)
			{
				for (int j = 0; j < tileSize; j++)
				{
					if (img.getPixel(x * tileSize + i, y * tileSize + j) != sf::Color::Transparent)
					{
						empty = false;
						break;
					}
				}
				if (!empty)
				{
					break;
				}
			}
			if (!empty)
			{
				sf::RectangleShape tile(sf::Vector2f(tileSize * 2, tileSize * 2));
				tile.setTexture(&texture);
				tile.setTextureRect(rect);
				tile.setPosition((tiles.size() % tilesPerRow) * (tileSize * 2 + 2), (tiles.size() / tilesPerRow) * (tileSize * 2 + 2));
				tile.setOutlineThickness(1);
				tile.setOutlineColor(sf::Color::Black);
				tiles.push_back(tile);
				tileToTextureIndex.push_back(y * yLimit + x);
			}
		}
	}*/
	ifstream file("Resources/ValidTextureData.jpg");
	if (file.is_open())
	{
		while (!file.eof())
		{
			int index;
			file >> index;
			tileToTextureIndex.push_back(index);
		}
		file.close();
	}
	for (int i = 0; i < tileToTextureIndex.size(); i++)
	{
		sf::IntRect rect((tileToTextureIndex[i] % yLimit) * tileSize, (tileToTextureIndex[i] / yLimit) * tileSize, tileSize, tileSize);
		sf::RectangleShape tile(sf::Vector2f(tileSize * 2, tileSize * 2));
		tile.setTexture(&texture);
		tile.setTextureRect(rect);
		tile.setPosition((tiles.size() % tilesPerRow) * (tileSize * 2 + 2), (tiles.size() / tilesPerRow) * (tileSize * 2 + 2));
		tile.setOutlineThickness(1);
		tile.setOutlineColor(sf::Color::Black);
		tiles.push_back(tile);
	}
}

void TilesEditor::draw(sf::RenderTarget& target) const
{
	target.draw(background);
	for (const auto& tile : tiles)
	{
		target.draw(tile);
	}
}

void TilesEditor::update(float deltaTime)
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		if (background.getGlobalBounds().contains(mousePosInUI))
		{
			blockClick = true;
		}
	}
}

void TilesEditor::handleEvent(sf::Event event)
{
	if (event.type == sf::Event::MouseButtonPressed && !blockClick)
	{
		if (event.mouseButton.button == sf::Mouse::Left)
		{
			if (background.getGlobalBounds().contains(mousePosInUI))
				blockClick = true;
			for (int i = 0; i < tiles.size(); i++)
			{
				if (tiles[i].getGlobalBounds().contains(mousePosInUI))
				{
					selectedTileIndex = i;
					indexOfSelectedTile = tileToTextureIndex[i];
					break;
				}
			}
		}
	}
	if (event.type == sf::Event::MouseWheelScrolled && !blockScroll)
	{
		if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel && background.getGlobalBounds().contains(mousePosInUI))
		{
			blockScroll = true;
			// move the tiles up or down
			if (event.mouseWheelScroll.delta > 0)
			{
				for (auto& tile : tiles)
				{
					tile.move(0, 20);
				}
			}
			else
			{
				for (auto& tile : tiles)
				{
					tile.move(0, -20);
				}
			}
		}
	}
}

TilesEditor::~TilesEditor()
{
	ofstream file("Resources/ValidTextureData.jpg");
	if (file.is_open())
	{
		for (int i = 0; i < tileToTextureIndex.size(); i++)
		{
			file << tileToTextureIndex[i] << " ";
		}
		file.close();
	}
}
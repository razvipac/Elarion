#pragma once
#include "SFML/Graphics.hpp"
class Tile {
private:
	sf::Texture tileTexture;
	int tileID;
	bool collision;
public:
	Tile(int tileID, bool collision, std::string path);
	int getTileID();
	sf::Texture* getTexture();
	bool getCollision();
};
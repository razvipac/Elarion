#include "Tile.h"
using namespace std;
using namespace sf;

Tile::Tile(int tileID, bool collision, string path) {
	this->tileID = tileID;
	this->collision = collision;
	tileTexture.loadFromFile(path);
}

int Tile::getTileID() {
	return tileID;
}

Texture* Tile::getTexture() {
	return &tileTexture;
}

bool Tile::getCollision() {
	return collision;
}
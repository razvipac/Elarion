#include "Tilemap.h"
#include <iostream>

using namespace std;
using namespace sf;

TileMap::TileMap() : width(256), height(256), tileSize(16) {
	level = new int[width * height];
	for (int i = 0; i < width * height; i++) {
		level[i] = 71;
	}
}
void TileMap::draw(RenderTarget& target, RenderStates states) const {
	// Apply the transform
	states.transform *= getTransform();

	// Apply the tileset texture
	states.texture = &tileset;

	// Draw the vertex array
	target.draw(vertices, states);
}

int TileMap::getWidth() const {
	return width;
}
int TileMap::getHeight() const {
	return height;
}
int TileMap::getTileSize() const {
	return tileSize;
}
const int* TileMap::getLevel() const {
	return level;
}
int TileMap::getTile(int x, int y) const {
	if (x < 0 || x >= width || y < 0 || y >= height)
		return -1;
	return level[x + y * width];
}
void TileMap::setTileSize(int tileSize) {
	this->tileSize = tileSize;
}
void TileMap::setLevel(int* level) {
	for (int i = 0; i < width * height; i++) {
		this->level[i] = level[i];
	}
	update();
}
void TileMap::setTile(int x, int y, int tile) {
	if (x < 0 || x >= width || y < 0 || y >= height)
		return;
	level[x + y * width] = tile;
	update();
}
bool TileMap::load(const string& tilesetPath) {
	// Load the tileset texture
	if (!tileset.loadFromFile(tilesetPath))
		return false;

	// Resize the vertex array to fit the level size
	vertices.setPrimitiveType(Quads);
	vertices.resize(width * height * 4);

	update();

	return true;
}
void TileMap::update() {
	// Populate the vertex array, with one quad per tile
	for (unsigned int i = 0; i < width; ++i)
		for (unsigned int j = 0; j < height; ++j) {
			// Get the current tile number
			int tileNumber = level[i + j * width];

			// Find its position in the tileset texture
			int tu = tileNumber % (tileset.getSize().x / tileSize);
			int tv = tileNumber / (tileset.getSize().x / tileSize);

			// Get a pointer to the current tile's quad
			Vertex* quad = &vertices[(i + j * width) * 4];

			// Define its 4 corners
			quad[0].position = Vector2f(i * tileSize, j * tileSize);
			quad[1].position = Vector2f((i + 1) * tileSize, j * tileSize);
			quad[2].position = Vector2f((i + 1) * tileSize, (j + 1) * tileSize);
			quad[3].position = Vector2f(i * tileSize, (j + 1) * tileSize);

			// Define its 4 texture coordinates
			quad[0].texCoords = Vector2f(tu * tileSize, tv * tileSize);
			quad[1].texCoords = Vector2f((tu + 1) * tileSize, tv * tileSize);
			quad[2].texCoords = Vector2f((tu + 1) * tileSize, (tv + 1) * tileSize);
			quad[3].texCoords = Vector2f(tu * tileSize, (tv + 1) * tileSize);
		}
}
TileMap::~TileMap() {
	delete[] level;
}
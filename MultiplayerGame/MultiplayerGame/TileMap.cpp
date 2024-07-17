#include "Tilemap.h"

using namespace std;
using namespace sf;

void TileMap::draw(RenderTarget& target, RenderStates states) const {
	// Apply the transform
	states.transform *= getTransform();

	// Apply the tileset texture
	states.texture = &tileset;

	// Draw the vertex array
	target.draw(vertices, states);
}

bool TileMap::load(const string& tilesetPath, Vector2u tileSize, const int* tiles, int width, int height) {
	// Load the tileset texture
	if (!tileset.loadFromFile(tilesetPath))
		return false;

	// Resize the vertex array to fit the level size
	vertices.setPrimitiveType(Quads);
	vertices.resize(width * height * 4);

	update(tiles, tileSize, width, height);

	return true;
}

void TileMap::update(const int* tiles, Vector2u tileSize, int width, int height) {
	// Populate the vertex array, with one quad per tile
	for (unsigned int i = 0; i < width; ++i)
		for (unsigned int j = 0; j < height; ++j) {
			// Get the current tile number
			int tileNumber = tiles[i + j * width];

			// Find its position in the tileset texture
			int tu = tileNumber % (tileset.getSize().x / tileSize.x);
			int tv = tileNumber / (tileset.getSize().x / tileSize.x);

			// Get a pointer to the current tile's quad
			Vertex* quad = &vertices[(i + j * width) * 4];

			// Define its 4 corners
			quad[0].position = Vector2f(i * tileSize.x, j * tileSize.y);
			quad[1].position = Vector2f((i + 1) * tileSize.x, j * tileSize.y);
			quad[2].position = Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);
			quad[3].position = Vector2f(i * tileSize.x, (j + 1) * tileSize.y);

			// Define its 4 texture coordinates
			quad[0].texCoords = Vector2f(tu * tileSize.x, tv * tileSize.y);
			quad[1].texCoords = Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
			quad[2].texCoords = Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
			quad[3].texCoords = Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
		}
}
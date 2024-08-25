#include "MapEditor.h"
#include <iostream>

using namespace std;
using namespace sf;

extern int indexOfSelectedTile;
extern Vector2f mousePosInWorld;

MapEditor::MapEditor() {
	tileMap.load("Resources/Tileset/spr_tileset_sunnysideworld_16px.png");
	pressedLastFrame = false;
}

void MapEditor::draw(RenderTarget& target) const {
	target.draw(tileMap);
}

void MapEditor::update(float deltaTime) {
	if (Mouse::isButtonPressed(Mouse::Left)) {
		int x = mousePosInWorld.x / tileMap.getTileSize();
		int y = mousePosInWorld.y / tileMap.getTileSize();
		//tileMap.setTile(x, y, selectedTileIndex);
		if(!pressedLastFrame)
			tileMap.setTile(x, y, indexOfSelectedTile);
		else {
			int x0 = lastMousePos.x / tileMap.getTileSize();
			int y0 = lastMousePos.y / tileMap.getTileSize();
			int dx = abs(x - x0);
			int dy = abs(y - y0);
			int sx = x0 < x ? 1 : -1;
			int sy = y0 < y ? 1 : -1;
			int err = dx - dy;
			while (true) {
				tileMap.setTile(x0, y0, indexOfSelectedTile);
				if (x0 == x && y0 == y)
					break;
				int e2 = 2 * err;
				if (e2 > -dy) {
					err -= dy;
					x0 += sx;
				}
				if (e2 < dx) {
					err += dx;
					y0 += sy;
				}
			}
		}
		pressedLastFrame = true;
	}
	else {
		pressedLastFrame = false;
	}

	lastMousePos = mousePosInWorld;
}

void MapEditor::handleEvent(Event event) {
}

MapEditor::~MapEditor() {
}
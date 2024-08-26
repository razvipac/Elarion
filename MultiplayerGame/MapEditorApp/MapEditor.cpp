#include "MapEditor.h"
#include <iostream>
#include "FileSelection.h"

using namespace std;
using namespace sf;

extern int indexOfSelectedTile;
extern Vector2f mousePosInWorld;
extern bool blockClick;
extern int selectedLayer;

MapEditor::MapEditor() {
	underneathLayer.load("Resources/Tileset/spr_tileset_sunnysideworld_16px.png");
	onTopLayer.load("Resources/Tileset/spr_tileset_sunnysideworld_16px.png");
	pressedLastFrame = false;
	int width = onTopLayer.getWidth();
	int height = onTopLayer.getHeight();
	for(int i = 0; i < width; i++)
		for(int j = 0; j < height; j++)
			onTopLayer.setTile(i, j, 0);
}

void MapEditor::draw(RenderTarget& target) const {
	target.draw(underneathLayer);
	target.draw(onTopLayer);
}

void MapEditor::update(float deltaTime) {
	if (Mouse::isButtonPressed(Mouse::Left) && !blockClick) {
		int x = mousePosInWorld.x / underneathLayer.getTileSize();
		int y = mousePosInWorld.y / underneathLayer.getTileSize();
		//tileMap.setTile(x, y, selectedTileIndex);
		if (!pressedLastFrame)
		{
			if(selectedLayer == 0)
				underneathLayer.setTile(x, y, indexOfSelectedTile);
			else
				onTopLayer.setTile(x, y, indexOfSelectedTile);
		}
		else {
			int x0 = lastMousePos.x / underneathLayer.getTileSize();
			int y0 = lastMousePos.y / underneathLayer.getTileSize();
			int dx = abs(x - x0);
			int dy = abs(y - y0);
			int sx = x0 < x ? 1 : -1;
			int sy = y0 < y ? 1 : -1;
			int err = dx - dy;
			while (true) {
				if(selectedLayer == 0)
					underneathLayer.setTile(x0, y0, indexOfSelectedTile);
				else
					onTopLayer.setTile(x0, y0, indexOfSelectedTile);
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
	// if CTRL+S is pressed, save the map
	if (event.type == Event::KeyReleased && event.key.code == Keyboard::S && Keyboard::isKeyPressed(Keyboard::LControl)) {
		string path = SaveFileDialog();
		ofstream file(path);
		underneathLayer.saveMapInfo(file);
		onTopLayer.saveMapInfo(file);
		file.close();
	}
	// if CTRL+O is pressed, load the map
	if (event.type == Event::KeyReleased && event.key.code == Keyboard::O && Keyboard::isKeyPressed(Keyboard::LControl)) {
		string path = OpenFileDialog();
		ifstream file(path);
		underneathLayer.loadMapInfo(file);
		onTopLayer.loadMapInfo(file);
		file.close();
	}
}

MapEditor::~MapEditor() {
}
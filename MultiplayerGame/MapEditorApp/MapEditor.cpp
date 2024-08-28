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
	for (int i = 0; i < width; i++)
		for (int j = 0; j < height; j++)
			onTopLayer.setTile(i, j, 0);
	undoLimit = 30;
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
			if (selectedLayer == 0)
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
				if (selectedLayer == 0)
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
	// if CTRL+Z is pressed, undo the last action
	if (event.type == Event::KeyReleased && event.key.code == Keyboard::Z && Keyboard::isKeyPressed(Keyboard::LControl)) {
		undo();
	}
	if (event.type == Event::MouseButtonPressed && event.key.code == Mouse::Left && !blockClick) {
		startAction(true);
	}
	if (event.type == Event::MouseButtonReleased && event.key.code == Mouse::Left && !blockClick) {
		checkForChanges();
	}
	// if CTRL+Y is pressed, redo the last action
	if (event.type == Event::KeyReleased && event.key.code == Keyboard::Y && Keyboard::isKeyPressed(Keyboard::LControl))
		redo();
}

void MapEditor::undo() {
	// if there are no actions to undo, return
	if (underneathLayerHistory.empty() || onTopLayerHistory.empty())
		return;
	// get the last action
	int* underneathLayerAction = underneathLayerHistory.back();
	int* onTopLayerAction = onTopLayerHistory.back();
	//
	startAction(false);
	// copy the action to the tile map
	underneathLayer.setLevel(underneathLayerAction);
	onTopLayer.setLevel(onTopLayerAction);
	// remove the action from the history
	underneathLayerHistory.pop_back();
	onTopLayerHistory.pop_back();

	delete[] underneathLayerAction;
	delete[] onTopLayerAction;
}

void MapEditor::redo() {
	// if there are no actions to redo, return
	if (underneathLayerFuture.empty() || onTopLayerFuture.empty())
		return;
	// get the last action
	int* underneathLayerAction = underneathLayerFuture.back();
	int* onTopLayerAction = onTopLayerFuture.back();
	//
	startAction(true);
	// copy the action to the tile map
	underneathLayer.setLevel(underneathLayerAction);
	onTopLayer.setLevel(onTopLayerAction);
	// remove the action from the history
	underneathLayerFuture.pop_back();
	onTopLayerFuture.pop_back();

	delete[] underneathLayerAction;
	delete[] onTopLayerAction;
}

void MapEditor::startAction(bool forUndo) {
	// create a new action
	int* underneathLayerAction = new int[underneathLayer.getWidth() * underneathLayer.getHeight()];
	int* onTopLayerAction = new int[onTopLayer.getWidth() * onTopLayer.getHeight()];
	// copy the tile map to the action
	const int* underneathLevel = underneathLayer.getLevel();
	const int* onTopLevel = onTopLayer.getLevel();
	for (int i = 0; i < underneathLayer.getWidth() * underneathLayer.getHeight(); i++) {
		underneathLayerAction[i] = underneathLevel[i];
		onTopLayerAction[i] = onTopLevel[i];
	}
	if (forUndo) {
		// add the action to the history
		underneathLayerHistory.push_back(underneathLayerAction);
		onTopLayerHistory.push_back(onTopLayerAction);
	}
	else {
		// add the action to the future
		underneathLayerFuture.push_back(underneathLayerAction);
		onTopLayerFuture.push_back(onTopLayerAction);
	}
}

void MapEditor::finishAction(bool changed) {
	if (changed) {
		if (underneathLayerHistory.size() > undoLimit) {
			delete[] underneathLayerHistory.front();
			underneathLayerHistory.pop_front();
		}
		if (onTopLayerHistory.size() > undoLimit) {
			delete[] onTopLayerHistory.front();
			onTopLayerHistory.pop_front();
		}
		underneathLayerFuture.clear();
		onTopLayerFuture.clear();
	}
	else {
		delete[] underneathLayerHistory.back();
		underneathLayerHistory.pop_back();
		delete[] onTopLayerHistory.back();
		onTopLayerHistory.pop_back();
	}
}

void MapEditor::checkForChanges() {
	// if there are no actions in the history, return
	if (underneathLayerHistory.empty() || onTopLayerHistory.empty())
		return;
	// get the last action
	int* underneathLayerAction = underneathLayerHistory.back();
	int* onTopLayerAction = onTopLayerHistory.back();
	// compare the tile map to the action
	const int* underneathLevel = underneathLayer.getLevel();
	const int* onTopLevel = onTopLayer.getLevel();
	bool changed = false;
	for (int i = 0; i < underneathLayer.getWidth() * underneathLayer.getHeight(); i++) {
		if (underneathLevel[i] != underneathLayerAction[i] || onTopLevel[i] != onTopLayerAction[i]) {
			changed = true;
			break;
		}
	}
	// if the tile map is different from the action, finish the action
	finishAction(changed);
}

MapEditor::~MapEditor() {
	while (!underneathLayerHistory.empty()) {
		delete[] underneathLayerHistory.back();
		underneathLayerHistory.pop_back();
	}
	while (!onTopLayerHistory.empty()) {
		delete[] onTopLayerHistory.back();
		onTopLayerHistory.pop_back();
	}
	while (!underneathLayerFuture.empty()) {
		delete[] underneathLayerFuture.back();
		underneathLayerFuture.pop_back();
	}
	while (!onTopLayerFuture.empty()) {
		delete[] onTopLayerFuture.back();
		onTopLayerFuture.pop_back();
	}
}
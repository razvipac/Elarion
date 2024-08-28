#pragma once
#include "TileMap.h"
#include <SFML/Graphics.hpp>
#include <list>

class MapEditor
{
private:
	TileMap underneathLayer;
	TileMap onTopLayer;
	bool pressedLastFrame = false;
	sf::Vector2f lastMousePos;
	std::list<int*> underneathLayerHistory;
	std::list<int*> onTopLayerHistory;
	int undoLimit;
	std::list<int*> underneathLayerFuture;
	std::list<int*> onTopLayerFuture;
public:
	MapEditor();
	void draw(sf::RenderTarget& target) const;
	void update(float deltaTime);
	void handleEvent(sf::Event event);
	void undo();
	void redo();
	void startAction(bool forUndo);
	void finishAction(bool changed);
	void checkForChanges();
	~MapEditor();
};
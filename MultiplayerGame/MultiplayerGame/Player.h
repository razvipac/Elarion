#pragma once
#include "SFML/Graphics.hpp"
#include "Inventory.h"
class Player
{
private:
	sf::RectangleShape player;
	float speed;
	int id;
	sf::Vector2f targetPosition;
	float timeSinceLastPacket;
	Inventory inventory;
public:
	Player(int id);
	void setPosition(sf::Vector2f position);
	void setTargetPosition(sf::Vector2f position);
	void drawPlayer(sf::RenderWindow& window);
	void update(float deltaTime);
	void setSpeed(float speed);
	void setId(int id);
	void setColor(sf::Color color);
	sf::Vector2f getPosition();
	int getId();
	void handleEvent(sf::Event event);
	int getSelectedItemId();
	void setSelectedItemId(int itemId);
};
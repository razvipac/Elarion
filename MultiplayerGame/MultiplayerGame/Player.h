#pragma once

#include "SFML/Graphics.hpp"
#include "Inventory.h"
#include "Animation.h"
#include "Animator.h"

class Player
{
private:
	sf::RectangleShape player;
	Animator playerAnimator;
	float speed;
	int id;
	sf::Vector2f targetPosition;
	float timeSinceLastPacket;
	Inventory inventory;
	Animation playerAnimation;
	float attackCooldown;
	float timeSinceLastAttack;

	sf::RectangleShape item;
	Animator itemAnimator;
public:
	Player(int id);
	void setPosition(sf::Vector2f position);
	void setTargetPosition(sf::Vector2f position);
	void drawPlayer(sf::RenderWindow& window) const;
	void drawInventory(sf::RenderWindow& window) const;
	void update(float deltaTime);
	void setSpeed(float speed);
	void setId(int id);
	void setColor(sf::Color color);
	sf::Vector2f getPosition();
	int getId();
	void handleEvent(sf::Event event, sf::RenderWindow& window);
	int getSelectedItemId();
	void setSelectedItemId(int itemId);
	void handleMouseClick(sf::Event event, sf::RenderWindow& window);
};
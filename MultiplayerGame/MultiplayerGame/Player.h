#pragma once

#include "Entity.h"
#include "Inventory.h"

class Player : public Entity
{
private:
	//sf::RectangleShape player;
	//Animator playerAnimator;
	//float speed;
	int id;
	sf::Vector2f targetPosition;
	float timeSinceLastPacket;
	Inventory inventory;
	//Animation playerAnimation;
	//float attackCooldown;
	//float timeSinceLastAttack;

	sf::RectangleShape item;
	Animator itemAnimator;
public:
	Player(int id);
	//void setPosition(sf::Vector2f position);
	void setTargetPosition(const sf::Vector2f& position);
	void draw(sf::RenderWindow& window) const override;
	void drawInventory(sf::RenderWindow& window) const;

	void derivedUpdate(float deltaTime) override;

	//void update(float deltaTime);
	//void setSpeed(float speed);
	void setId(int id);
	//void setColor(sf::Color color);
	//sf::Vector2f getPosition();
	int getId() const;
	void handleEvent(sf::Event& event, sf::RenderWindow& window);
	int getSelectedItemId() const;
	void setSelectedItemId(int itemId);
	//void handleMouseClick(sf::Event event, sf::RenderWindow& window);
};
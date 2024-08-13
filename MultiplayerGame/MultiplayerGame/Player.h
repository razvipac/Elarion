#pragma once

#include "Entity.h"
#include "Inventory.h"

class Player : public Entity
{
private:
	sf::Vector2f targetPosition;
	float timeSinceLastPacket;
	Inventory inventory;

	sf::RectangleShape item;
	Animator itemAnimator;

	//sf::Text healthText;
	//Bar healthBar;
public:
	Player(int id);
	void setTargetPosition(const sf::Vector2f& position);
	void draw(sf::RenderWindow& window) const override;
	void drawInventory(sf::RenderWindow& window) const;

	void derivedUpdate(float deltaTime) override;

	void attackVisual() override;

	void takeDamage(float damage) override;

	void handleEvent(sf::Event& event, sf::RenderWindow& window);
	int getSelectedItemId() const;
	void setSelectedItemId(int itemId);

	void die() override;
};
#pragma once
#include "EntityStats.h"
#include "SFML/Graphics.hpp"
#include "Animator.h"
#include "Bar.h"
#include <iostream>

// #include "IDamageable.h" //EntityStats contains all the methods from IDamageable, so it might be useless

// Entity class inherits from EntityStats
// Entity class is an abstract class and serves as the base class for all entities in the game (including the player)
class Entity : public EntityStats
{
protected:
	int id;
	float timeSinceLastAttack;
	float timeSinceLastRegen;
	float timeSinceLastHit;
	sf::RectangleShape entity;
	Animator entityAnimator;
	Bar healthBar;
public:
	Entity(const std::string& animatorPath, int id);
	Entity(const std::string& animatorPath, int id, float health, float armor, float healthRegen, float timeBeforeRegenStarts, float invulnerability, float speed, float attackCooldown, float attackRange, float damage);
	
	// Update method is called every frame
	virtual void update(float deltaTime);
	// Derived update method is called every frame after the update method
	// It is a pure virtual method and must be implemented by the derived class
	virtual void derivedUpdate(float deltaTime) = 0;

	virtual void draw(sf::RenderWindow& window) const;

	void setId(int id);
	int getId() const;

	void move(float x, float y);
	void move(sf::Vector2f direction);

	void setPosition(const sf::Vector2f& position);
	void setPosition(float x, float y);
	const sf::Vector2f& getPosition() const;

	void attack(Entity* target);
	virtual void attackVisual();

	virtual void takeDamage(float damage);

	void die() override;

	~Entity() {}
};
#include "EntityStats.h"
#include <iostream>
#include <cmath>

EntityStats::EntityStats() {
	health = 100;
	baseMaxHealth = 100;
	baseArmor = 0;
	baseHealthRegen = 1;
	timeBeforeRegenStarts = 5;

	invulnerabilityTimeAfterHit = 0.4f;

	baseSpeed = 150;

	baseAttackCooldown = 0.5;
	baseAttackRange = 50;
	baseDamage = 10;
}

EntityStats::EntityStats(float health, float armor, float healthRegen, float timeBeforeRegenStarts, float invulnerability, float speed, float attackCooldown, float attackRange, float damage) {
	this->health = health;
	baseMaxHealth = health;
	baseArmor = armor;
	baseHealthRegen = healthRegen;
	this->timeBeforeRegenStarts = timeBeforeRegenStarts;

	invulnerabilityTimeAfterHit = invulnerability;

	baseSpeed = speed;

	baseAttackCooldown = attackCooldown;
	baseAttackRange = attackRange;
	baseDamage = damage;
}

float EntityStats::getHealth() const {
	return health;
}

float EntityStats::getMaxHealth() const {
	return baseMaxHealth;
}

float EntityStats::getArmor() const {
	return baseArmor;
}

float EntityStats::getHealthRegen() const {
	return baseHealthRegen;
}

float EntityStats::getSpeed() const {
	return baseSpeed;
}

float EntityStats::getAttackCooldown() const {
	return baseAttackCooldown;
}

float EntityStats::getAttackRange() const {
	return baseAttackRange;
}

float EntityStats::getDamage() const {
	return baseDamage;
}

void EntityStats::setHealth(float health) {
	this->health = health;
}

void EntityStats::setMaxHealth(float maxHealth) {
	baseMaxHealth = maxHealth;
}

void EntityStats::setArmor(float armor) {
	baseArmor = armor;
}

void EntityStats::setHealthRegen(float healthRegen) {
	baseHealthRegen = healthRegen;
}

void EntityStats::setSpeed(float speed) {
	baseSpeed = speed;
}

void EntityStats::setAttackCooldown(float attackCooldown) {
	baseAttackCooldown = attackCooldown;
}

void EntityStats::setAttackRange(float attackRange) {
	baseAttackRange = attackRange;
}

void EntityStats::setDamage(float damage) {
	baseDamage = damage;
}

void EntityStats::takeDamage(float damage) {
	float damageMultiplier = 1 - baseArmor / 100;
	damage *= damageMultiplier;

	health -= damage;
	if (health <= 0) {
		die();
	}
}

void EntityStats::heal(float health) {
	this->health += health;
	if (this->health > baseMaxHealth) {
		this->health = baseMaxHealth;
	}
}

void EntityStats::die() {
	std::cout << "Entity died" << std::endl;
}

void EntityStats::resetStats() {
	health = baseMaxHealth;
}
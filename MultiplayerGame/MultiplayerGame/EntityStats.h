#pragma once

class EntityStats
{
protected:
	float health;
	float baseMaxHealth;
	float baseArmor;
	float baseHealthRegen;
	float timeBeforeRegenStarts;

	float baseSpeed;

	float baseAttackCooldown;
	float baseAttackRange;
	float baseDamage;
public:
	EntityStats();
	EntityStats(float health, float armor, float healthRegen, float timeBeforeRegenStarts, float speed, float attackCooldown, float attackRange, float damage);
	float getHealth() const;
	float getMaxHealth() const;
	float getArmor() const;
	float getHealthRegen() const;
	float getSpeed() const;
	float getAttackCooldown() const;
	float getAttackRange() const;
	float getDamage() const;

	void setHealth(float health);
	void setMaxHealth(float maxHealth);
	void setArmor(float armor);
	void setHealthRegen(float healthRegen);
	void setSpeed(float speed);
	void setAttackCooldown(float attackCooldown);
	void setAttackRange(float attackRange);
	void setDamage(float damage);

	void takeDamage(float damage);
	void heal(float health);
	void die();
	void resetStats();
};
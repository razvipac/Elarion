#pragma once

class IDamageable
{
	public:
	virtual void takeDamage(float damage) = 0;
	virtual void heal(float health) = 0;
	virtual float getHealth() const = 0;
	virtual float getMaxHealth() const = 0;
	virtual void die() = 0;
};
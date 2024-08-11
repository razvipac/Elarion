#include "Entity.h"
#include "TextureManager.h"

using namespace std;
using namespace sf;

Entity::Entity(const string& animatorPath) : entityAnimator(entity)
{
	entity.setSize(Vector2f(96, 64));
	entity.setScale(3, 3);
	entity.setOrigin(entity.getSize() / 2.f);
	entity.setPosition(Vector2f(0, 0));

	entityAnimator.loadAnimator(animatorPath);
	entity.setTextureRect(entityAnimator.getFrame());

	string textureName = entityAnimator.getCurrentState()->getPath();
	//Remove the .anim extension
	textureName = textureName.substr(0, textureName.size() - 5);
	entity.setTexture(&TextureManager::getInstance().getRef(textureName));

	timeSinceLastAttack = baseAttackCooldown;
	timeSinceLastRegen = timeBeforeRegenStarts;
	timeSinceLastHit = invulnerabilityTimeAfterHit;
}

Entity::Entity(const string& animatorPath, float health, float armor, float healthRegen, float timeBeforeRegenStarts, float invulnerability, float speed, float attackCooldown, float attackRange, float damage) : EntityStats(health, armor, healthRegen, timeBeforeRegenStarts, invulnerability, speed, attackCooldown, attackRange, damage), entityAnimator(entity)
{
	entity.setSize(Vector2f(96, 64));
	entity.setScale(3, 3);
	entity.setOrigin(entity.getSize() / 2.f);
	entity.setPosition(Vector2f(0, 0));

	entityAnimator.loadAnimator(animatorPath);
	entity.setTextureRect(entityAnimator.getFrame());
	entity.setTexture(&TextureManager::getInstance().getRef(entityAnimator.getCurrentState()->getPath()));

	timeSinceLastAttack = baseAttackCooldown;
	timeSinceLastRegen = timeBeforeRegenStarts;
	timeSinceLastHit = invulnerabilityTimeAfterHit;
}

void Entity::update(float deltaTime)
{
	if (health <= 0)
		return;

	timeSinceLastAttack += deltaTime;
	timeSinceLastRegen += deltaTime;
	timeSinceLastHit += deltaTime;
	if (timeSinceLastRegen >= timeBeforeRegenStarts)
	{
		heal(baseHealthRegen * deltaTime);
	}
	if(timeSinceLastHit >= invulnerabilityTimeAfterHit)
		entityAnimator.setBool("Hurt", false);
	derivedUpdate(deltaTime);
	entityAnimator.update(deltaTime);
	entity.setTextureRect(entityAnimator.getFrame());
}
void Entity::draw(RenderWindow& window) const
{
	if(health <= 0)
		return;
	window.draw(entity);
}

void Entity::move(float x, float y)
{
	entity.move(x, y);
}
void Entity::move(Vector2f direction)
{
	entity.move(direction);
}

void Entity::setPosition(const Vector2f& position)
{
	entity.setPosition(position);
}
void Entity::setPosition(float x, float y)
{
	entity.setPosition(x, y);
}
const Vector2f& Entity::getPosition() const
{
	return entity.getPosition();
}

void Entity::attack(Entity* target)
{
	if (timeSinceLastAttack >= baseAttackCooldown)
	{
		float distance = sqrt(pow(target->entity.getPosition().x - entity.getPosition().x, 2) + pow(target->entity.getPosition().y - entity.getPosition().y, 2));
		if (distance <= baseAttackRange)
		{
			cout<<"Attacking "<<endl;
			target->takeDamage(baseDamage);
			timeSinceLastAttack = 0;
		}
	}
}

void Entity::takeDamage(float damage)
{
	if (timeSinceLastHit >= invulnerabilityTimeAfterHit)
	{
		entityAnimator.setBool("Hurt", true);
		timeSinceLastHit = 0;

		EntityStats::takeDamage(damage);
	}
}
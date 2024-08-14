#include "Entity.h"
#include "TextureManager.h"
#include "NetworkManager.h"
#include "Utility.h"

using namespace std;
using namespace sf;

Entity::Entity(const string& animatorPath, int id) : entityAnimator(entity)
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

	this->id = id;
}

Entity::Entity(const string& animatorPath, int id, float health, float armor, float healthRegen, float timeBeforeRegenStarts, float invulnerability, float speed, float attackCooldown, float attackRange, float damage) : EntityStats(health, armor, healthRegen, timeBeforeRegenStarts, invulnerability, speed, attackCooldown, attackRange, damage), entityAnimator(entity)
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

	this->id = id;
}

void Entity::update(float deltaTime)
{
	if (health <= 0) {
		entityAnimator.setBool("Death", true);
		entityAnimator.update(deltaTime);
		entity.setTextureRect(entityAnimator.getFrame());
		derivedUpdate(deltaTime);
		return;
	}

	healthBar.setValue(health);
	healthBar.setPosition(entity.getPosition() - Vector2f(healthBar.getSize().x / 2, entity.getSize().y / 2 + 10));
	healthBar.update(deltaTime);

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
	healthBar.draw(window);
}

void Entity::setId(int id)
{
	this->id = id;
}
int Entity::getId() const
{
	return id;
}

void Entity::move(float x, float y)
{
	entity.move(x, y);
	healthBar.setPosition(entity.getPosition() - Vector2f(healthBar.getSize().x / 2, entity.getSize().y / 2 + 10));
}
void Entity::move(Vector2f direction)
{
	entity.move(direction);
	healthBar.setPosition(entity.getPosition() - Vector2f(healthBar.getSize().x / 2, entity.getSize().y / 2 + 10));
}

void Entity::setPosition(const Vector2f& position)
{
	entity.setPosition(position);
	healthBar.setPosition(entity.getPosition() - Vector2f(healthBar.getSize().x / 2, entity.getSize().y / 2 + 10));
}
void Entity::setPosition(float x, float y)
{
	entity.setPosition(x, y);
	healthBar.setPosition(entity.getPosition() - Vector2f(healthBar.getSize().x / 2, entity.getSize().y / 2 + 10));
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

			char buffer[13];
			//packHitData(buffer, 5, id, target->getId(), baseDamage);
			packHitData(buffer, 5, -20, id, baseDamage);
			NetworkManager::getInstance().sendPacket(buffer, 13);

			attackVisual();
		}
	}
}

void Entity::attackVisual()
{
	entityAnimator.setBool("Hurt", false);
	entityAnimator.setBool("Attack", true);
	timeSinceLastAttack = 0;
}

void Entity::takeDamage(float damage)
{
	if (timeSinceLastHit >= invulnerabilityTimeAfterHit)
	{
		cout<<"Taking damage "<<endl;
		entityAnimator.setBool("Hurt", true);
		timeSinceLastHit = 0;

		EntityStats::takeDamage(damage);

		//healthBar.setValue(health);
	}
}

void Entity::die()
{
	entityAnimator.setBool("Hurt", false);
	entityAnimator.setBool("Attack", false);
	entityAnimator.setFloat("Speed", 0);
	health = 0;
	entityAnimator.setBool("Death", true);
}
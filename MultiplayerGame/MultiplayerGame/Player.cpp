#include "Player.h"
#include <iostream>	
#include "TextureManager.h"
#include <cmath>
#include <map>

using namespace sf;
using namespace std;

extern int CLIENTID;
extern int tps;
extern Vector2f mousePosInWorld;
extern map<int, Player*> playerMap;
extern Font font;

Player::Player(int id) : Entity("Resources/Animations/Player.animator", id), itemAnimator(item) {
	timeSinceLastPacket = 0;

	item.setSize(entity.getSize());
	item.setScale(entity.getScale());
	item.setOrigin(entity.getOrigin());
	item.setPosition(entity.getPosition());


	/*Animation playerAnimation;
	playerAnimation.createAnimation("PlayerAttack", 0.5f, true);
	playerAnimation.createAnimation("ItemAttack", 0.5f, true);*/

	//Load animator
	itemAnimator.loadAnimator("Resources/Animations/Item.animator");


	item.setTextureRect(itemAnimator.getFrame());
	item.setTexture(&TextureManager::getInstance().getRef("ItemIdle"));


	healthText.setFont(font);
	healthText.setCharacterSize(20);
	healthText.setFillColor(Color::White);
	healthText.setOutlineColor(Color::Black);
	healthText.setOutlineThickness(1);
	healthText.setPosition(entity.getPosition().x - 30, entity.getPosition().y - 50);
	healthText.setString(to_string((int)health));
}
void Player::setTargetPosition(const Vector2f& position) {
	targetPosition = position;
	timeSinceLastPacket = 0;
	// if the target position is within one unit of the player's position, set the animator's speed to 0
	if (abs(targetPosition.x - entity.getPosition().x) < 1 && abs(targetPosition.y - entity.getPosition().y) < 1)
		entityAnimator.setFloat("Speed", 0.0f);
}
void Player::handleEvent(Event& event, RenderWindow& window) {
	if (id == CLIENTID) {
		inventory.handleEvent(event);
	}
}
void Player::derivedUpdate(float deltaTime) {

	if(health <= 0)
	{
		itemAnimator.update(deltaTime);
		item.setTextureRect(itemAnimator.getFrame());
		return;
	}

	if (timeSinceLastHit >= invulnerabilityTimeAfterHit)
		itemAnimator.setBool("Hurt", false);

	//timeSinceLastAttack += deltaTime;
	float currentSpeed = baseSpeed;
	float animatorSpeed = 0.0f;
	bool isAttacking = false;

	// Movement logic
	if (id == CLIENTID) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift)) {
			currentSpeed *= 1.5f;
		}

		if (Mouse::isButtonPressed(Mouse::Left)) {
			if (timeSinceLastAttack >= baseAttackCooldown) {
				//Handle the attack
				for (auto& p : playerMap) {
					if (p.first != id) {
						if (p.second->entity.getGlobalBounds().contains(mousePosInWorld)) {
							cout << "Mouse in bounds.\n";
							attack(p.second);
							break;
						}
					}
				}

				//Handle the visual
				timeSinceLastAttack = 0;
				isAttacking = true;
			}
		}

		Vector2f movementVector(0, 0);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
			movementVector.y -= currentSpeed * deltaTime;
			animatorSpeed = currentSpeed;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
			movementVector.y += currentSpeed * deltaTime;
			animatorSpeed = currentSpeed;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
			movementVector.x -= currentSpeed * deltaTime;
			animatorSpeed = currentSpeed;
			if (entity.getScale().x > 0)
			{
				entity.setScale(-entity.getScale().x, entity.getScale().y);
				item.setScale(-item.getScale().x, item.getScale().y);
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			movementVector.x += currentSpeed * deltaTime;
			animatorSpeed = currentSpeed;
			if (entity.getScale().x < 0)
			{
				entity.setScale(-entity.getScale().x, entity.getScale().y);
				item.setScale(-item.getScale().x, item.getScale().y);
			}
		}
		if (movementVector.x != 0 || movementVector.y != 0)
		{
			float distance = sqrt(movementVector.x * movementVector.x + movementVector.y * movementVector.y);
			movementVector = movementVector / distance;
			entity.move(movementVector * currentSpeed * deltaTime);
			item.setPosition(entity.getPosition());
			healthText.setPosition(entity.getPosition().x - 30, entity.getPosition().y - 50);
		}
		entityAnimator.setFloat("Speed", animatorSpeed);
		itemAnimator.setFloat("Speed", animatorSpeed);
		if (isAttacking || timeSinceLastAttack >= baseAttackCooldown)
		{
			entityAnimator.setBool("Attack", isAttacking);
			itemAnimator.setBool("Attack", isAttacking);
		}
	}
	else {
		// when time since last packet is 1 / tps, the player will be at the target position
		float timeLeft = 1.0f / tps - timeSinceLastPacket;

		if (timeLeft < deltaTime) {
			entity.setPosition(targetPosition);
			item.setPosition(targetPosition);
			healthText.setPosition(entity.getPosition().x - 30, entity.getPosition().y - 50);
			timeSinceLastPacket = 0;
		}
		else if (!(abs(targetPosition.x - entity.getPosition().x) < 1 && abs(targetPosition.y - entity.getPosition().y) < 1)) {
			Vector2f currentPosition = entity.getPosition();
			Vector2f direction = targetPosition - currentPosition;
			Vector2f directionPerSecond = direction / timeLeft;
			Vector2f newPosition = currentPosition + directionPerSecond * deltaTime;

			entity.setPosition(newPosition);
			item.setPosition(newPosition);
			healthText.setPosition(entity.getPosition().x - 30, entity.getPosition().y - 50);
			timeSinceLastPacket += deltaTime; // might need to increase it beforehand

			currentSpeed = sqrt(directionPerSecond.x * directionPerSecond.x + directionPerSecond.y * directionPerSecond.y);
			animatorSpeed = currentSpeed;

			// if we move the player to the right and the player's scale is negative, flip the player
			if (directionPerSecond.x > 0 && entity.getScale().x < 0) {
				entity.setScale(-entity.getScale().x, entity.getScale().y);
				item.setScale(-item.getScale().x, item.getScale().y);
			}
			// if we move the player to the left and the player's scale is positive, flip the player
			else if (directionPerSecond.x < 0 && entity.getScale().x > 0)
			{
				entity.setScale(-entity.getScale().x, entity.getScale().y);
				item.setScale(-item.getScale().x, item.getScale().y);
			}

			entityAnimator.setFloat("Speed", animatorSpeed);

			if (timeSinceLastAttack >= baseAttackCooldown)
			{
				entityAnimator.setBool("Attack", false);
				itemAnimator.setBool("Attack", false);
			}
		}
	}
	itemAnimator.update(deltaTime);
	item.setTextureRect(itemAnimator.getFrame());
}

void Player::attackVisual() {
	itemAnimator.setBool("Attack", true);
	Entity::attackVisual();
}

void Player::takeDamage(float damage) {
	if (timeSinceLastHit >= invulnerabilityTimeAfterHit)
	{
		cout << "Taking damage " << endl;
		entityAnimator.setBool("Hurt", true);
		itemAnimator.setBool("Hurt", true);
		timeSinceLastHit = 0;

		EntityStats::takeDamage(damage);
	}
}

void Player::draw(RenderWindow& window) const {
	window.draw(entity); //Player is still being drawn after dying
	window.draw(item);
	if (health > 0)
		window.draw(healthText);
}
void Player::drawInventory(RenderWindow& window) const {
	if (id == CLIENTID) {
		inventory.drawInventory(window);
	}
}
int Player::getSelectedItemId() const {
	return inventory.getItemID(inventory.getSelectedSlot());
}
void Player::setSelectedItemId(int itemId) {
	inventory.changeItem(inventory.getSelectedSlot(), itemId, 1);
}

void Player::die() {
	itemAnimator.setBool("Death", true);
	Entity::die();
}
#include "Player.h"
#include <iostream>	
#include "TextureManager.h"
#include <cmath>
#include <map>
#include "TileMap.h"
#include "Plant.h"

using namespace sf;
using namespace std;

extern int CLIENTID;
extern int tps;
extern Vector2f mousePosInWorld;
extern map<int, Player*> playerMap;
extern Font font;
extern TileMap tileMap;
extern TileMap tileMap2;
extern vector<Plant*> plants;

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

	//healthText.setFont(font);
	/*healthText.setCharacterSize(20);
	healthText.setFillColor(Color::White);
	healthText.setOutlineColor(Color::Black);
	healthText.setOutlineThickness(1);
	healthText.setPosition(entity.getPosition().x - 30, entity.getPosition().y - 50);
	healthText.setString(to_string((int)health));*/

	// generate a random position on the map, but make sure there is not a tile in the tileMap2
	Vector2f position;
	do {
		position = Vector2f(rand() % (int)(tileMap.getWidth() * tileMap.getTileSize() * tileMap.getScale().x), rand() % (int)(tileMap.getHeight() * tileMap.getTileSize() * tileMap.getScale().y));
	} while (tileMap2.getTile(position.x / tileMap.getTileSize() / tileMap.getScale().x, position.y / tileMap.getTileSize() / tileMap.getScale().y) != 0);
	entity.setPosition(position);
	item.setPosition(position);

	// give the player a random seed
	inventory.addItem(2, 1);
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
		if (event.type == Event::MouseButtonPressed)
		{
			if (event.mouseButton.button == Mouse::Left)
			{
				interact();
			}
		}
	}
}
void Player::derivedUpdate(float deltaTime) {

	if (health <= 0)
	{
		itemAnimator.setBool("Death", true);
		itemAnimator.update(deltaTime);
		item.setTextureRect(itemAnimator.getFrame());
		return;
	}

	//healthText.setString(to_string((int)health));

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
			//healthText.setPosition(entity.getPosition().x - 30, entity.getPosition().y - 50);
			// dont let the player go out of bounds
			if (entity.getPosition().x < 16)
			{
				entity.setPosition(16, entity.getPosition().y);
				item.setPosition(entity.getPosition());
				//healthText.setPosition(entity.getPosition().x - 30, entity.getPosition().y - 50);
			}
			if (entity.getPosition().y < 40)
			{
				entity.setPosition(entity.getPosition().x, 40);
				item.setPosition(entity.getPosition());
				//healthText.setPosition(entity.getPosition().x - 30, entity.getPosition().y - 50);
			}
			if (entity.getPosition().x > tileMap.getWidth() * tileMap.getTileSize() * tileMap.getScale().x - 16)
			{
				entity.setPosition(tileMap.getWidth() * tileMap.getTileSize() * tileMap.getScale().x - 16, entity.getPosition().y);
				item.setPosition(entity.getPosition());
				//healthText.setPosition(entity.getPosition().x - 30, entity.getPosition().y - 50);
			}
			if (entity.getPosition().y > tileMap.getHeight() * tileMap.getTileSize() * tileMap.getScale().y - 16)
			{
				entity.setPosition(entity.getPosition().x, tileMap.getHeight() * tileMap.getTileSize() * tileMap.getScale().y - 16);
				item.setPosition(entity.getPosition());
				//healthText.setPosition(entity.getPosition().x - 30, entity.getPosition().y - 50);
			}
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
			//healthText.setPosition(entity.getPosition().x - 30, entity.getPosition().y - 50);
			timeSinceLastPacket = 0;
		}
		else if (!(abs(targetPosition.x - entity.getPosition().x) < 1 && abs(targetPosition.y - entity.getPosition().y) < 1)) {
			Vector2f currentPosition = entity.getPosition();
			Vector2f direction = targetPosition - currentPosition;
			Vector2f directionPerSecond = direction / timeLeft;
			Vector2f newPosition = currentPosition + directionPerSecond * deltaTime;

			entity.setPosition(newPosition);
			item.setPosition(newPosition);
			//healthText.setPosition(entity.getPosition().x - 30, entity.getPosition().y - 50);
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
		}
		if (timeSinceLastAttack >= baseAttackCooldown)
		{
			entityAnimator.setBool("Attack", false);
			itemAnimator.setBool("Attack", false);
		}
	}
	itemAnimator.update(deltaTime);
	item.setTextureRect(itemAnimator.getFrame());
}

void Player::attackVisual() {
	itemAnimator.setBool("Hurt", false);
	itemAnimator.setBool("Attack", true);
	Entity::attackVisual();
}

void Player::takeDamage(float damage) {
	if (timeSinceLastHit >= invulnerabilityTimeAfterHit)
	{
		cout << "Taking damage " << endl;
		if (!entityAnimator.getBool("Attack"))
		{
			entityAnimator.setBool("Hurt", true);
			itemAnimator.setBool("Hurt", true);
		}
		timeSinceLastHit = 0;
		EntityStats::takeDamage(damage);
		//healthText.setString(to_string((int)health));
	}
}

void Player::draw(RenderWindow& window) const {
	window.draw(entity); //Player is still being drawn after dying
	window.draw(item);
	if (health > 0)
		healthBar.draw(window);
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
	inventory.changeItem(inventory.getSelectedSlot(), itemId, 2);
}

void Player::interact() {
	// find what tile we are hovering over
	Vector2i tilePos = Vector2i(mousePosInWorld / (float)tileMap.getTileSize() / 3.f);
	// if we are too far away from the tile, return
	if (abs(entity.getPosition().x - tilePos.x * tileMap.getTileSize() * 3 - 24) > 72 || abs(entity.getPosition().y - tilePos.y * tileMap.getTileSize() * 3 - 32) > 72)
		return;
	// find the tile id of the tile we are hovering over
	int tileId = tileMap.getTile(tilePos.x, tilePos.y);
	int selectedItem = getSelectedItemId();
	if (tileId == 67 && selectedItem % 2 == 0 && selectedItem > 0 && selectedItem < 23)
	{
		bool ok = true;
		for(int i = 0; i < plants.size(); i++)
			if (plants[i]->getPosition() == Vector2f(tilePos.x * tileMap.getTileSize() * 3 + 24, tilePos.y * tileMap.getTileSize() * 3 + 32))
				ok = false;
		if (!ok)
			return;

		// if the tile is a plant and the selected item is a seed, plant the seed
		Plant* plant = new Plant((selectedItem - 1) / 2);
		plant->setPosition(tilePos.x * tileMap.getTileSize() * 3 + 24, tilePos.y * tileMap.getTileSize() * 3 + 32);
		plants.push_back(plant);
		inventory.removeItem(selectedItem, 1);
	}
	cout << "TileId: " << tileId << endl;
}

void Player::die() {
	itemAnimator.setBool("Hurt", false);
	itemAnimator.setBool("Attack", false);
	itemAnimator.setFloat("Speed", 0);
	itemAnimator.setBool("Death", true);
	Entity::die();
}

void Player::addItems(int itemID, int quantity) {
	inventory.addItem(itemID, quantity);
}
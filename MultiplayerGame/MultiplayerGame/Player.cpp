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

Player::Player(int id) : Entity("Resources/Animations/Player.animator"), itemAnimator(item) {
	this->id = id;
	timeSinceLastPacket = 0;
	/*player.setSize(Vector2f(96, 64));
	player.setScale(3, 3);
	player.setOrigin(player.getSize() / 2.f);
	player.setPosition(Vector2f(0, 0));
	speed = 150.f;
	attackCooldown = 0.5f;
	timeSinceLastAttack = 0;*/

	/*playerAnimation.createAnimation("PlayerIdle", 0.7f, true);
	playerAnimation.createAnimation("PlayerWalk", 0.7f, true);
	playerAnimation.createAnimation("PlayerRun", 0.7f, true);
	playerAnimation.createAnimation("PlayerHurt", 0.7f, false);
	playerAnimation.createAnimation("PlayerAttack", 0.5f, false);*/

	item.setSize(entity.getSize());
	item.setScale(entity.getScale());
	item.setOrigin(entity.getOrigin());
	item.setPosition(entity.getPosition());

	//playerAnimation.createAnimation("ItemIdle", 0.7f, true);
	//playerAnimation.createAnimation("ItemWalk", 0.7f, true);
	//playerAnimation.createAnimation("ItemRun", 0.7f, true);
	//playerAnimation.createAnimation("ItemHurt", 0.7f, false);
	//playerAnimation.createAnimation("ItemAttack", 0.5f, false);

	//Load animator
	//playerAnimator.loadAnimator("Resources/Animations/Player.animator");
	itemAnimator.loadAnimator("Resources/Animations/Item.animator");

	/*player.setTextureRect(playerAnimator.getFrame());
	player.setTexture(&TextureManager::getInstance().getRef("PlayerIdle"));*/

	item.setTextureRect(itemAnimator.getFrame());
	item.setTexture(&TextureManager::getInstance().getRef("ItemIdle"));

	//player.setOutlineColor(Color::Red);
	//player.setOutlineThickness(1);
}
//void Player::setPosition(Vector2f position) {
//	player.setPosition(position);
//}
void Player::setTargetPosition(const Vector2f& position) {
	targetPosition = position;
	timeSinceLastPacket = 0;
	// if the target position is within one unit of the player's position, set the animator's speed to 0
	if (abs(targetPosition.x - entity.getPosition().x) < 1 && abs(targetPosition.y - entity.getPosition().y) < 1)
		entityAnimator.setFloat("Speed", 0.0f);
}
void Player::setId(int id) {
	this->id = id;
}
//void Player::setSpeed(float speed) {
//	this->speed = speed;
//}
//void Player::setColor(Color color) {
//	player.setFillColor(color);
//}
void Player::handleEvent(Event& event, RenderWindow& window) {
	if (id == CLIENTID) {
		inventory.handleEvent(event);
		//handleMouseClick(event, window);
	}
}
void Player::derivedUpdate(float deltaTime) {
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
			timeSinceLastPacket = 0;
		}
		else if (!(abs(targetPosition.x - entity.getPosition().x) < 1 && abs(targetPosition.y - entity.getPosition().y) < 1)) {
			Vector2f currentPosition = entity.getPosition();
			Vector2f direction = targetPosition - currentPosition;
			Vector2f directionPerSecond = direction / timeLeft;
			Vector2f newPosition = currentPosition + directionPerSecond * deltaTime;

			entity.setPosition(newPosition);
			timeSinceLastPacket += deltaTime; // might need to increase it beforehand

			currentSpeed = sqrt(directionPerSecond.x * directionPerSecond.x + directionPerSecond.y * directionPerSecond.y);
			animatorSpeed = currentSpeed;

			// if we move the player to the right and the player's scale is negative, flip the player
			if (directionPerSecond.x > 0 && entity.getScale().x < 0)
				entity.setScale(-entity.getScale().x, entity.getScale().y);
			// if we move the player to the left and the player's scale is positive, flip the player
			else if (directionPerSecond.x < 0 && entity.getScale().x > 0)
				entity.setScale(-entity.getScale().x, entity.getScale().y);

			entityAnimator.setFloat("Speed", animatorSpeed);
		}
	}
	//entityAnimator.update(deltaTime);
	//entity.setTextureRect(entityAnimator.getFrame());
	itemAnimator.update(deltaTime);
	item.setTextureRect(itemAnimator.getFrame());
}
void Player::draw(RenderWindow& window) const {
	window.draw(entity); //Player is still being drawn after dying
	window.draw(item);
}
void Player::drawInventory(RenderWindow& window) const {
	if (id == CLIENTID) {
		inventory.drawInventory(window);
	}
}
//Vector2f Player::getPosition() {
//	return player.getPosition();
//}
int Player::getId() const {
	return id;
}
int Player::getSelectedItemId() const {
	return inventory.getItemID(inventory.getSelectedSlot());
}
void Player::setSelectedItemId(int itemId) {
	inventory.changeItem(inventory.getSelectedSlot(), itemId, 1);
}

//void Player::handleMouseClick(Event event, RenderWindow& window) {
//	if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
//		Vector2f clickPosition = window.mapPixelToCoords(Vector2i(event.mouseButton.x, event.mouseButton.y));
//		Vector2f playerPosition = player.getPosition();
//		float distance = static_cast<float>(sqrt(pow(clickPosition.x - playerPosition.x, 2) + pow(clickPosition.y - playerPosition.y, 2)));
//
//		/*cout << "Click Position: (" << clickPosition.x << ", " << clickPosition.y << ")" << endl;
//		cout << "Player Position: (" << playerPosition.x << ", " << playerPosition.y << ")" << endl;
//		cout << "Distance: " << distance << endl;
//
//		cout << "This is the player's x position " << player.getScale().x << "\n";*/
//
//		if (distance <= 75.0f)
//			cout << "Click is in range distance from the player." << endl;
//	}
//}
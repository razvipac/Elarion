#include "Player.h"
#include <iostream>	
#include "TextureManager.h"
#include <cmath>

using namespace sf;
using namespace std;

extern int CLIENTID;
extern int tps;

Player::Player(int id) : playerAnimator(player) {
	player.setSize(Vector2f(65, 16));
	player.setScale(3, 3);
	player.setOrigin(8, 8);
	player.setPosition(Vector2f(0, 0));
	speed = 150.f;
	this->id = id;
	timeSinceLastPacket = 0;

	//playerHurt.addFrame(IntRect());

	/*playerHurt.loadAnimation("PlayerAttack.anim");
	playerHurt.setDuration(0.4f);
	playerHurt.saveAnimation("PlayerAttack.anim");*/

	/*playerHurt.loadAnimation("Resources/Animations/PlayerHurt.anim");
	playerHurt.setDuration(0.4f);
	playerHurt.saveAnimation("Resources/Animations/PlayerHurt.anim");*/

	//Load animator
	playerAnimator.loadAnimator("Resources/Animations/Player.animator");

	player.setTextureRect(playerAnimator.getFrame());
	player.setTexture(&TextureManager::getInstance().getRef("PlayerIdle"));

	player.setOutlineColor(Color::Red);
	player.setOutlineThickness(1);
}
void Player::setPosition(Vector2f position) {
	player.setPosition(position);
}
void Player::setTargetPosition(Vector2f position) {
	targetPosition = position;
	timeSinceLastPacket = 0;
	// if the target position is within one unit of the player's position, set the animator's speed to 0
	if (abs(targetPosition.x - player.getPosition().x) < 1 && abs(targetPosition.y - player.getPosition().y) < 1)
		playerAnimator.setFloat("Speed", 0.0f);
}
void Player::setId(int id) {
	this->id = id;
}
void Player::setSpeed(float speed) {
	this->speed = speed;
}
void Player::setColor(Color color) {
	player.setFillColor(color);
}
void Player::handleEvent(Event event, RenderWindow& window) {
	if (id == CLIENTID) {
		inventory.handleEvent(event);
		handleMouseClick(event, window);
	}
}
void Player::update(float deltaTime) {
	float currentSpeed = speed;
	float animatorSpeed = 0.0f;

	// Movement logic
	if (id == CLIENTID) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift)) {
			currentSpeed *= 1.5f;
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
			if (player.getScale().x > 0)
				player.setScale(-player.getScale().x, player.getScale().y);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			movementVector.x += currentSpeed * deltaTime;
			animatorSpeed = currentSpeed;
			if (player.getScale().x < 0)
				player.setScale(-player.getScale().x, player.getScale().y);
		}
		if (movementVector.x != 0 || movementVector.y != 0)
		{
			float distance = sqrt(movementVector.x * movementVector.x + movementVector.y * movementVector.y);
			movementVector = movementVector / distance;
			player.move(movementVector * currentSpeed * deltaTime);
		}
		playerAnimator.setFloat("Speed", animatorSpeed);
	}
	else {
		// when time since last packet is 1 / tps, the player will be at the target position
		float timeLeft = 1.0f / tps - timeSinceLastPacket;

		if (timeLeft < deltaTime) {
			player.setPosition(targetPosition);
			timeSinceLastPacket = 0;
		}
		else if (!(abs(targetPosition.x - player.getPosition().x) < 1 && abs(targetPosition.y - player.getPosition().y) < 1)) {
			Vector2f currentPosition = player.getPosition();
			Vector2f direction = targetPosition - currentPosition;
			Vector2f directionPerSecond = direction / timeLeft;
			Vector2f newPosition = currentPosition + directionPerSecond * deltaTime;

			player.setPosition(newPosition);
			timeSinceLastPacket += deltaTime;

			currentSpeed = sqrt(directionPerSecond.x * directionPerSecond.x + directionPerSecond.y * directionPerSecond.y);
			animatorSpeed = currentSpeed;

			// if we move the player to the right and the player's scale is negative, flip the player
			if (directionPerSecond.x > 0 && player.getScale().x < 0)
				player.setScale(-player.getScale().x, player.getScale().y);
			// if we move the player to the left and the player's scale is positive, flip the player
			else if (directionPerSecond.x < 0 && player.getScale().x > 0)
				player.setScale(-player.getScale().x, player.getScale().y);

			playerAnimator.setFloat("Speed", animatorSpeed);
		}
	}
	playerAnimator.update(deltaTime);
	player.setTextureRect(playerAnimator.getFrame());
}
void Player::drawPlayer(RenderWindow& window) const {
	window.draw(player);
}
void Player::drawInventory(RenderWindow& window) const {
	if (id == CLIENTID) {
		inventory.drawInventory(window);
	}
}
Vector2f Player::getPosition() {
	return player.getPosition();
}
int Player::getId() {
	return id;
}
int Player::getSelectedItemId() {
	return inventory.getItemID(inventory.getSelectedSlot());
}
void Player::setSelectedItemId(int itemId) {
	inventory.changeItem(inventory.getSelectedSlot(), itemId, 1);
}

void Player::handleMouseClick(Event event, RenderWindow& window) {
	if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
		Vector2f clickPosition = window.mapPixelToCoords(Vector2i(event.mouseButton.x, event.mouseButton.y));
		Vector2f playerPosition = player.getPosition();
		float distance = static_cast<float>(sqrt(pow(clickPosition.x - playerPosition.x, 2) + pow(clickPosition.y - playerPosition.y, 2)));

		/*cout << "Click Position: (" << clickPosition.x << ", " << clickPosition.y << ")" << endl;
		cout << "Player Position: (" << playerPosition.x << ", " << playerPosition.y << ")" << endl;
		cout << "Distance: " << distance << endl;

		cout << "This is the player's x position " << player.getScale().x << "\n";*/

		if (distance <= 75.0f)
			cout << "Click is in range distance from the player." << endl;
	}
}
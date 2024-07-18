#include "Player.h"
#include <iostream>	
#include "TextureManager.h"

using namespace sf;
using namespace std;

extern int CLIENTID;

Player::Player(int id) {
	player.setSize(Vector2f(16, 16));
	player.setScale(3, 3);
	//player.setFillColor(Color::Red);
	player.setOrigin(8, 8);
	player.setPosition(Vector2f(0, 0));
	/*player.setOutlineColor(Color::Black);
	player.setOutlineThickness(2);*/
	speed = 200.f;
	this->id = id;
	//if(id == CLIENTID)
	//	player.setFillColor(Color::Green);
	timeSinceLastPacket = 0;

	idleAnimation.loadAnimation("Resources/Animations/PlayerIdle.anim");
	player.setTextureRect(idleAnimation.getCurrentFrame());
	player.setTexture(&TextureManager::getInstance().getRef("PlayerIdle"));

	walkAnimation.loadAnimation("Resources/Animations/PlayerWalk.anim");

}

void Player::setPosition(Vector2f position) {
	player.setPosition(position);
}
void Player::setTargetPosition(Vector2f position) {
	targetPosition = position;
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

void Player::handleEvent(Event event) {
	if (id == CLIENTID) {
		inventory.handleEvent(event);
	}
}
void Player::update(float deltaTime) {
	//cout << id << " " << CLIENTID << "\n";

	bool walking = false;

	if (id != CLIENTID) {
		timeSinceLastPacket += deltaTime;
		Vector2f currentPosition = player.getPosition();
		Vector2f direction = targetPosition - currentPosition;
		float distance = sqrt(direction.x * direction.x + direction.y * direction.y);
		if (distance < 1) {
			player.setPosition(targetPosition);
			timeSinceLastPacket = 0;
		}
		else {
			walking = true;
			Vector2f unitVector = direction / distance;
			Vector2f movement = unitVector * speed * deltaTime;
			if (movement.x > 0 && currentPosition.x + movement.x > targetPosition.x)
				player.setPosition(targetPosition);
			else if (movement.x < 0 && currentPosition.x + movement.x < targetPosition.x)
				player.setPosition(targetPosition);
			else if (movement.y > 0 && currentPosition.y + movement.y > targetPosition.y)
				player.setPosition(targetPosition);
			else if (movement.y < 0 && currentPosition.y + movement.y < targetPosition.y)
				player.setPosition(targetPosition);
			else
				player.move(movement);
		}
	}
	else {
		if (Keyboard::isKeyPressed(Keyboard::W)) {
			player.move(0, -speed * deltaTime);
			walking = true;
		}
		if (Keyboard::isKeyPressed(Keyboard::S)) {
			player.move(0, speed * deltaTime);
			walking = true;
		}
		if (Keyboard::isKeyPressed(Keyboard::A)) {
			player.move(-speed * deltaTime, 0);
			walking = true;
			if(player.getScale().x > 0)
				player.setScale(-player.getScale().x, player.getScale().y);
		}
		if (Keyboard::isKeyPressed(Keyboard::D)) {
			player.move(speed * deltaTime, 0);
			walking = true;
			if (player.getScale().x < 0)
				player.setScale(-player.getScale().x, player.getScale().y);
		}
	}

	if (walking) {
		if (walkAnimation.update(deltaTime)) {
			player.setTextureRect(walkAnimation.getCurrentFrame());
		}
		if (player.getTexture() != &TextureManager::getInstance().getRef("PlayerWalk")) {
			player.setTexture(&TextureManager::getInstance().getRef("PlayerWalk"));
			idleAnimation.resetTime();
		}
	}
	else {
		if (idleAnimation.update(deltaTime)) {
			player.setTextureRect(idleAnimation.getCurrentFrame());
		}
		if (player.getTexture() != &TextureManager::getInstance().getRef("PlayerIdle")) {
			player.setTexture(&TextureManager::getInstance().getRef("PlayerIdle"));
			walkAnimation.resetTime();
		}
	}
}
void Player::drawPlayer(RenderWindow& window) {
	window.draw(player);

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
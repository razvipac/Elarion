#include "Player.h"
#include "Client.h"
#include <iostream>	
using namespace sf;
using namespace std;

Player::Player(int id) {
	player.setSize(Vector2f(50, 50));
	player.setFillColor(Color::Red);
	player.setPosition(Vector2f(0, 0));
	speed = 200.f;
	this->id = id;
	if(id == CLIENTID)
		player.setFillColor(Color::Green);
	timeSinceLastPacket = 0;
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

void Player::drawPlayer(RenderWindow& window) {
	window.draw(player);

	if (id == CLIENTID) {
		inventory.drawInventory(window);
	}
}

void Player::update(float deltaTime) {
	//cout << id << " " << CLIENTID << "\n";
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
		if(Keyboard::isKeyPressed(Keyboard::W))
			player.move(0, -speed * deltaTime);
		if (Keyboard::isKeyPressed(Keyboard::S))
			player.move(0, speed * deltaTime);
		if(Keyboard::isKeyPressed(Keyboard::A))
			player.move(-speed * deltaTime, 0);
		if(Keyboard::isKeyPressed(Keyboard::D))
			player.move(speed * deltaTime, 0);
	}
}

void Player::setSpeed(float speed) {
	this->speed = speed;
}

void Player::setColor(Color color) {
	player.setFillColor(color);
}

Vector2f Player::getPosition() {
	return player.getPosition();
}

int Player::getId() {
	return id;
}

void Player::handleEvent(Event event) {
	if (id == CLIENTID) {
		inventory.handleEvent(event);
	}
}

int Player::getSelectedItemId() {
	return inventory.getItemID(inventory.getSelectedSlot());
}

void Player::setSelectedItemId(int itemId) {
	inventory.changeItem(inventory.getSelectedSlot(), itemId, 1);
}
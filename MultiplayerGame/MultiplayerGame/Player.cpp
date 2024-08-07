#include "Player.h"
#include <iostream>	
#include "TextureManager.h"

using namespace sf;
using namespace std;

extern int CLIENTID;
extern int tps;

Player::Player(int id) : playerAnimator(player) {
	player.setSize(Vector2f(16, 16));
	player.setScale(3, 3);
	//player.setFillColor(Color::Red);
	player.setOrigin(8, 8);
	player.setPosition(Vector2f(0, 0));
	/*player.setOutlineColor(Color::Black);
	player.setOutlineThickness(2);*/
	speed = 150.f;
	this->id = id;
	//if(id == CLIENTID)
	//	player.setFillColor(Color::Green);
	timeSinceLastPacket = 0;

    //Load animator
    playerAnimator.loadAnimator("Resources/Animations/Player.animator");

	//idleAnimation.loadAnimation("Resources/Animations/PlayerIdle.anim");
	player.setTextureRect(playerAnimator.getFrame());
	player.setTexture(&TextureManager::getInstance().getRef("PlayerIdle"));

	//walkAnimation.loadAnimation("Resources/Animations/PlayerWalk.anim");

	// I want to load and save another animation for the player
	//runAnimation.loadAnimation("Resources/Animations/PlayerRun.anim");
	
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
    //bool walking = false;
    //bool running = false;

    float currentSpeed = speed;
    float animatorSpeed = 0.0f;

    // Movement logic
    if (id == CLIENTID) {
        // Determine if the player is running based on Shift key being held
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift)) {
            //running = true;
            currentSpeed *= 1.5f;
        }

        //if (running) {
        //    currentSpeed *= 1.5; // Double the speed when running
        //}

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            player.move(0, -currentSpeed * deltaTime);
            animatorSpeed = currentSpeed;
            //walking = true;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            player.move(0, currentSpeed * deltaTime);
            animatorSpeed = currentSpeed;
            //walking = true;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            player.move(-currentSpeed * deltaTime, 0);
            animatorSpeed = currentSpeed;
            //walking = true;
            if (player.getScale().x > 0)
                player.setScale(-player.getScale().x, player.getScale().y);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            player.move(currentSpeed * deltaTime, 0);
            animatorSpeed = currentSpeed;
            //walking = true;
            if (player.getScale().x < 0)
                player.setScale(-player.getScale().x, player.getScale().y);
        }
    }
    else {
        // when time since last packet is 1 / tps, the player will be at the target position
        float timeLeft = 1.0f / tps - timeSinceLastPacket;
        if (timeLeft < deltaTime) {
			player.setPosition(targetPosition);
			timeSinceLastPacket = 0;
		}
		else {
			Vector2f currentPosition = player.getPosition();
			Vector2f direction = targetPosition - currentPosition;
            Vector2f directionPerSecond = direction / timeLeft;
            Vector2f newPosition = currentPosition + directionPerSecond * deltaTime;
            player.setPosition(newPosition);
            timeSinceLastPacket += deltaTime;
            currentSpeed = sqrt(directionPerSecond.x * directionPerSecond.x + directionPerSecond.y * directionPerSecond.y);
            animatorSpeed = currentSpeed;
        }
        //timeSinceLastPacket += deltaTime;
        

  //      Vector2f currentPosition = player.getPosition();
  //      Vector2f direction = targetPosition - currentPosition;
  //      float distance = sqrt(direction.x * direction.x + direction.y * direction.y);
  //      if (distance < 1) {
  //          player.setPosition(targetPosition);
  //          timeSinceLastPacket = 0;
  //      }
  //      else {
		//	//walking = true;
		//	Vector2f unitVector = direction / distance;
		//	Vector2f movement = unitVector * currentSpeed * deltaTime;
		//	if (movement.x > 0 && currentPosition.x + movement.x > targetPosition.x)
		//		player.setPosition(targetPosition);
		//	else if (movement.x < 0 && currentPosition.x + movement.x < targetPosition.x)
		//		player.setPosition(targetPosition);
		//	else if (movement.y > 0 && currentPosition.y + movement.y > targetPosition.y)
		//		player.setPosition(targetPosition);
		//	else if (movement.y < 0 && currentPosition.y + movement.y < targetPosition.y)
		//		player.setPosition(targetPosition);
		//	else
		//		player.move(movement);

  //          //if the player moved to the right and the player's scale is negative, flip the player
  //          if (movement.x > 0 && player.getScale().x < 0)
		//		player.setScale(-player.getScale().x, player.getScale().y);
  //          //if the player moved to the left and the player's scale is positive, flip the player
		//	else if (movement.x < 0 && player.getScale().x > 0)
  //              player.setScale(-player.getScale().x, player.getScale().y);
		//}
    }

    // Update animations based on movement state
    //if (running && walking) {
    //    if (runAnimation.update(deltaTime)) {
    //        player.setTextureRect(runAnimation.getCurrentFrame());
    //    }
    //    if (player.getTexture() != &TextureManager::getInstance().getRef("PlayerRun")) {
    //        player.setTexture(&TextureManager::getInstance().getRef("PlayerRun"));
    //        walkAnimation.resetTime(); // Reset walk animation time if switching to run
    //        idleAnimation.resetTime(); // Reset idle animation time if switching to run
    //    }
    //}
    //else if (walking) {
    //    if (walkAnimation.update(deltaTime)) {
    //        player.setTextureRect(walkAnimation.getCurrentFrame());
    //    }
    //    if (player.getTexture() != &TextureManager::getInstance().getRef("PlayerWalk")) {
    //        player.setTexture(&TextureManager::getInstance().getRef("PlayerWalk"));
    //        idleAnimation.resetTime(); // Reset idle animation time if switching to walk
    //    }
    //}
    //else {
    //    if (idleAnimation.update(deltaTime)) {
    //        player.setTextureRect(idleAnimation.getCurrentFrame());
    //    }
    //    if (player.getTexture() != &TextureManager::getInstance().getRef("PlayerIdle")) {
    //        player.setTexture(&TextureManager::getInstance().getRef("PlayerIdle"));
    //        walkAnimation.resetTime(); // Reset walk animation time if switching to idle
    //        runAnimation.resetTime(); // Reset run animation time if switching to idle
    //    }
    //}

    // Update the player's animator
    //cout << "Speed: " << animatorSpeed << '\n';
    playerAnimator.setFloat("Speed", animatorSpeed);
    playerAnimator.update(deltaTime);
    player.setTextureRect(playerAnimator.getFrame());
}


//void Player::update(float deltaTime) {
//	//cout << id << " " << CLIENTID << "\n";
//
//	bool walking = false;
//	bool running = false;
//
	//if (id != CLIENTID) {
	//	timeSinceLastPacket += deltaTime;
	//	Vector2f currentPosition = player.getPosition();
	//	Vector2f direction = targetPosition - currentPosition;
	//	float distance = sqrt(direction.x * direction.x + direction.y * direction.y);
	//	if (distance < 1) {
	//		player.setPosition(targetPosition);
	//		timeSinceLastPacket = 0;
	//	}
//		else {
//			walking = true;
//			Vector2f unitVector = direction / distance;
//			Vector2f movement = unitVector * speed * deltaTime;
//			if (movement.x > 0 && currentPosition.x + movement.x > targetPosition.x)
//				player.setPosition(targetPosition);
//			else if (movement.x < 0 && currentPosition.x + movement.x < targetPosition.x)
//				player.setPosition(targetPosition);
//			else if (movement.y > 0 && currentPosition.y + movement.y > targetPosition.y)
//				player.setPosition(targetPosition);
//			else if (movement.y < 0 && currentPosition.y + movement.y < targetPosition.y)
//				player.setPosition(targetPosition);
//			else
//				player.move(movement);
//		}
//	}
//	else {
//		if (Keyboard::isKeyPressed(Keyboard::W)) {
//			player.move(0, -speed * deltaTime);
//			walking = true;
//		}
//		if (Keyboard::isKeyPressed(Keyboard::S)) {
//			player.move(0, speed * deltaTime);
//			walking = true;
//		}
//		if (Keyboard::isKeyPressed(Keyboard::A)) {
//			player.move(-speed * deltaTime, 0);
//			walking = true;
//			if (player.getScale().x > 0)
//				player.setScale(-player.getScale().x, player.getScale().y);
//		}
//		if (Keyboard::isKeyPressed(Keyboard::D)) {
//			player.move(speed * deltaTime, 0);
//			walking = true;
//			if (player.getScale().x < 0)
//				player.setScale(-player.getScale().x, player.getScale().y);
//		}
//		// Check for running (Shift key held down)
//		if (Keyboard::isKeyPressed(Keyboard::LShift) || Keyboard::isKeyPressed(Keyboard::RShift)) {
//			running = walking; // Only run if already walking
//			walking = false; // Ensure walking is not true when running
//			speed *= 2;
//		}
//	}
//
//	if (running) {
//		if (runAnimation.update(deltaTime)) {
//			player.setTextureRect(runAnimation.getCurrentFrame());
//		}
//		if (player.getTexture() != &TextureManager::getInstance().getRef("PlayerRun")) {
//			player.setTexture(&TextureManager::getInstance().getRef("PlayerRun"));
//			idleAnimation.resetTime();
//		}
//	}
//	else if (walking) {
//		if (walkAnimation.update(deltaTime)) {
//			player.setTextureRect(walkAnimation.getCurrentFrame());
//		}
//		if (player.getTexture() != &TextureManager::getInstance().getRef("PlayerWalk")) {
//			player.setTexture(&TextureManager::getInstance().getRef("PlayerWalk"));
//			idleAnimation.resetTime();
//		}
//	}
//	else {
//		if (idleAnimation.update(deltaTime)) {
//			player.setTextureRect(idleAnimation.getCurrentFrame());
//		}
//		if (player.getTexture() != &TextureManager::getInstance().getRef("PlayerIdle")) {
//			player.setTexture(&TextureManager::getInstance().getRef("PlayerIdle"));
//			walkAnimation.resetTime();
//		}
//	}
//}

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
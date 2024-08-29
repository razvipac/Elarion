#include "Plant.h"
#include "TextureManager.h"
#include <cmath>

using namespace std;
using namespace sf;

extern Vector2f mousePosInWorld;

float distance(Vector2f a, Vector2f b) {
	return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

IntRect operator+(const IntRect& a, const IntRect& b) {
	return IntRect(a.left + b.left, a.top + b.top, a.width + b.width, a.height + b.height);
}

Plant::Plant(int plantType) {

	this->plantType = plantType;

	plant.setSize(sf::Vector2f(growthStages[0].width, growthStages[0].height));
	plant.setScale(3, 3);
	plant.setOrigin(plant.getSize().x / 2, plant.getSize().y * 0.5f);
	plant.setPosition(0, 0);
	plant.setTexture(&TextureManager::getInstance().getRef("TextureAtlas"));
	plant.setTextureRect(growthStages[0] + IntRect(16 * plantType, 0, 0, 0));
	currentStage = 0;
	growthTime = 7;
	growthTimer = 0;

	markedForDeletion = false;
}

void Plant::draw(sf::RenderWindow& window) {
	window.draw(plant);
}

void Plant::grow() {
	if (currentStage < growthStages.size() - 1) {
		currentStage++;
		plant.setTextureRect(growthStages[currentStage] + IntRect(16 * plantType, 0, 0, 0));
		plant.setSize(sf::Vector2f(growthStages[currentStage].width, growthStages[currentStage].height));
		if(currentStage == 2) plant.setOrigin(plant.getSize().x / 2, plant.getSize().y * 0.75f);
		cout << plant.getSize().y << endl;
	}
}

void Plant::update(float deltaTime) {
	growthTimer += deltaTime;
	if (growthTimer >= growthTime) {
		grow();
		growthTimer = 0;
	}
}

void Plant::handleEvent(sf::Event& event, Vector2f playerPosition) {
	// if the player clicks on the plant and the plant is fully grown, delete the plant only if I am 72 pixels away from the plant
	if (event.type == sf::Event::MouseButtonPressed) {
		if (event.mouseButton.button == sf::Mouse::Left) {
			if (plant.getGlobalBounds().contains(mousePosInWorld.x, mousePosInWorld.y) && distance(plant.getPosition(), playerPosition) <= 72) {
				if (currentStage == growthStages.size() - 1) {
					markForDeletion();
				}
			}
		}
	}
}

void Plant::setGrowthStages(std::vector<sf::IntRect>& growthStages) {
	Plant::growthStages = growthStages;
}
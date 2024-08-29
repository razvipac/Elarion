#pragma once
#include "SFML/Graphics.hpp"
#include <vector>

class Plant {
private:
	sf::RectangleShape plant;
	static std::vector<sf::IntRect> growthStages;
	int currentStage;
	float growthTime;
	float growthTimer;
	int plantType;
	bool markedForDeletion;
public:
	Plant(int plantType);
	void draw(sf::RenderWindow& window);
	void grow();
	void update(float deltaTime);
	void handleEvent(sf::Event& event, sf::Vector2f playerPosition);
	void setPosition(float x, float y) { plant.setPosition(x, y); }
	static void setGrowthStages(std::vector<sf::IntRect>& growthStages);
	bool isMarkedForDeletion() { return markedForDeletion; }
	void markForDeletion() { markedForDeletion = true; }
};
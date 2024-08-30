#pragma once
#include "SFML/Graphics.hpp"
#include <iostream>
class Item {
private:
	sf::IntRect textureRect;
	std::string name;
	int maxStackSize;
	int maxDurability;
	int transformationId;
public:
	Item(std::string name, sf::IntRect textureRect, int maxStackSize, int maxDurability, int transformationId);
	std::string getName();
	sf::IntRect& getTexture();
	int getMaxStackSize();
	int getMaxDurability();
	int getTransformationId();
};
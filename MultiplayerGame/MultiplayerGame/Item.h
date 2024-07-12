#pragma once
#include "SFML/Graphics.hpp"
#include <iostream>
class Item {
private:
	std::string name;
	sf::Texture texture;
	int maxStackSize;
	int maxDurability;
	int transformationId;
public:
	Item(std::string name, std::string texturePath, int maxStackSize, int maxDurability, int transformationId);
	std::string getName();
	sf::Texture* getTexture();
	int getMaxStackSize();
	int getMaxDurability();
	int getTransformationId();
};
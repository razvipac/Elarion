#include "Item.h"
using namespace std;
using namespace sf;

Item::Item(string name, string texturePath, int maxStackSize, int maxDurability, int transformationId) {
	this->name = name;
	if(texturePath != "")
		this->texture.loadFromFile(texturePath);
	this->maxStackSize = maxStackSize;
	this->maxDurability = maxDurability;
	this->transformationId = transformationId;
}
string Item::getName() {
	return name;
}
Texture* Item::getTexture() {
	return &texture;
}
int Item::getMaxStackSize() {
	return maxStackSize;
}
int Item::getMaxDurability() {
	return maxDurability;
}
int Item::getTransformationId() {
	return transformationId;
}

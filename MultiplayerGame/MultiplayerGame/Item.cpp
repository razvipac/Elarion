#include "Item.h"

using namespace std;
using namespace sf;

Item::Item(string name, IntRect textureRect, int maxStackSize, int maxDurability, int transformationId) {
	this->name = name;
	this->textureRect = textureRect;
	this->maxStackSize = maxStackSize;
	this->maxDurability = maxDurability;
	this->transformationId = transformationId;
}
string Item::getName() {
	return name;
}
IntRect& Item::getTexture() {
	return textureRect;
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

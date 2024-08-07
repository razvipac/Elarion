#include "ItemSlot.h"

using namespace std;
using namespace sf;

vector<Item> ItemSlot::items;

ItemSlot::ItemSlot(Vector2f position) {
	itemID = 0;
	quantity = 0;
	durability = 0;
	slot.setSize(Vector2f(50, 50));
	slot.setFillColor(Color::White);
	slot.setPosition(position);
	slot.setOutlineColor(Color::Black);
	slot.setOutlineThickness(4);
	itemIcon.setPosition(position);
	itemIcon.setFillColor(Color::White);
	itemIcon.setSize(Vector2f(50, 50));

}
void ItemSlot::setItem(int itemID) {
	if (itemID < 0 || itemID >= items.size())
		itemID = 0;
	this->itemID = itemID;
	itemIcon.setSize(Vector2f(50, 50));
	itemIcon.setTexture(items[itemID].getTexture());
}
void ItemSlot::setQuantity(int quantity) {
	this->quantity = quantity;

	if (quantity <= 0) {
		setItem(0);
		this->quantity = 0;
		this->durability = 0;
	}

	if (quantity > items[itemID].getMaxStackSize())
		this->quantity = items[itemID].getMaxStackSize();
}
void ItemSlot::setDurability(int durability) {
	if (durability > items[itemID].getMaxDurability())
		durability = items[itemID].getMaxDurability();
	this->durability = durability;
	if (durability <= 0) {
		setItem(items[itemID].getTransformationId());
		this->durability = 0;
	}
}
int ItemSlot::getItemID() {
	return itemID;
}
int ItemSlot::getQuantity() {
	return quantity;
}
int ItemSlot::getDurability() {
	return durability;
}
void ItemSlot::drawSlot(RenderWindow& window) {
	window.draw(slot);
	window.draw(itemIcon);
}
void ItemSlot::loadItems() {
	items.push_back(Item("Empty", "", 0, 0, 0));
	items.push_back(Item("Empty Cup", "Resources/Textures/Cup_Empty.png", 1, 0, 0));
	items.push_back(Item("Cup of Water", "Resources/Textures/Cup_Water.png", 1, 1, 1));
}
Item& ItemSlot::getItem(int itemID) {
	if (itemID < 0 || itemID >= items.size())
		itemID = 0;
	return items[itemID];
}
void ItemSlot::changeSlotOutlineColor(Color color) {
	slot.setOutlineColor(color);
}
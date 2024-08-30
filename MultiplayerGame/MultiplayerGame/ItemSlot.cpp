#include "ItemSlot.h"
#include "TextureManager.h"

using namespace std;
using namespace sf;

vector<Item> ItemSlot::items;
extern Font font;

ItemSlot::ItemSlot(Vector2f position) {
	itemID = 0;
	quantity = 0;
	durability = 0;
	slot.setSize(Vector2f(50, 50));
	slot.setFillColor(Color(128, 128, 128, 200));
	slot.setPosition(position);
	slot.setOutlineColor(Color::Black);
	slot.setOutlineThickness(4);
	itemIcon.setPosition(position);
	itemIcon.setFillColor(Color::White);
	itemIcon.setSize(Vector2f(50, 50));
	itemIcon.setTexture(&TextureManager::getInstance().getRef("TextureAtlas"));
	setItem(0);
	quantityText.setFont(font);
	quantityText.setCharacterSize(25);
	quantityText.setFillColor(Color::White);
	quantityText.setOutlineColor(Color::Black);
	quantityText.setOutlineThickness(1);
	quantityText.setPosition(position.x + 50, position.y + 45);
	// set the origin to the right bottom corner of the text
	quantityText.setOrigin(quantityText.getLocalBounds().width, quantityText.getLocalBounds().height);
}
void ItemSlot::setItem(int itemID) {
	if (itemID < 0 || itemID >= items.size())
		itemID = 0;
	this->itemID = itemID;
	itemIcon.setSize(Vector2f(50, 50));
	itemIcon.setTextureRect(items[itemID].getTexture());
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
	quantityText.setString(to_string(this->quantity));
	quantityText.setOrigin(quantityText.getLocalBounds().width, quantityText.getLocalBounds().height);
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
int ItemSlot::getItemID() const {
	return itemID;
}
int ItemSlot::getQuantity() const {
	return quantity;
}
int ItemSlot::getDurability() const {
	return durability;
}
void ItemSlot::drawSlot(RenderWindow& window) const {
	window.draw(slot);
	window.draw(itemIcon);
	if(quantity > 1 && itemID != 0)
		window.draw(quantityText);
}
void ItemSlot::loadItems() {
	items.push_back(Item("Empty", IntRect(0, 0, 16, 16), 0, 0, 0));
	items.push_back(Item("Carrot", IntRect(816, 160, 16, 16), 64, 0, 0));
	items.push_back(Item("Carrot Seed", IntRect(816, 176, 16, 16), 64, 0, 0));
	items.push_back(Item("Cauliflower", IntRect(832, 160, 16, 16), 64, 0, 0));
	items.push_back(Item("Cauliflower seed", IntRect(832, 176, 16, 16), 64, 0, 0));
	items.push_back(Item("Pumpkin", IntRect(848, 160, 16, 32), 64, 0, 0));
	items.push_back(Item("Pumpkin seed", IntRect(848, 176, 16, 32), 64, 0, 0));
	items.push_back(Item("Sunflower", IntRect(864, 160, 16, 16), 64, 0, 0));
	items.push_back(Item("Sunflower seed", IntRect(864, 176, 16, 16), 64, 0, 0));
	items.push_back(Item("Radish", IntRect(880, 160, 16, 16), 64, 0, 0));
	items.push_back(Item("Radish seed", IntRect(880, 176, 16, 16), 64, 0, 0));
	items.push_back(Item("Parsnip", IntRect(896, 160, 16, 16), 64, 0, 0));
	items.push_back(Item("Parsnip seed", IntRect(896, 176, 16, 16), 64, 0, 0));
	items.push_back(Item("Potato", IntRect(912, 160, 16, 16), 64, 0, 0));
	items.push_back(Item("Potato seed", IntRect(912, 176, 16, 16), 64, 0, 0));
	items.push_back(Item("Turnip", IntRect(928, 160, 16, 16), 64, 0, 0));
	items.push_back(Item("Turnip seed", IntRect(928, 176, 16, 16), 64, 0, 0));
	items.push_back(Item("Beet", IntRect(944, 160, 16, 16), 64, 0, 0));
	items.push_back(Item("Beet seed", IntRect(944, 176, 16, 16), 64, 0, 0));
	items.push_back(Item("Wheat", IntRect(960, 160, 16, 16), 64, 0, 0));
	items.push_back(Item("Wheat seed", IntRect(960, 176, 16, 16), 64, 0, 0));
	items.push_back(Item("Cabbage", IntRect(976, 160, 16, 16), 64, 0, 0));
	items.push_back(Item("Cabbage seed", IntRect(976, 176, 16, 16), 64, 0, 0));
}
Item& ItemSlot::getItem(int itemID) {
	if (itemID < 0 || itemID >= items.size())
		itemID = 0;
	return items[itemID];
}
void ItemSlot::changeSlotOutlineColor(Color color) {
	slot.setOutlineColor(color);
}
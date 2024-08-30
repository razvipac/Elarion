#pragma once
#include "Item.h"
#include <vector>
#include "SFML/Graphics.hpp"

enum ItemNames {
	EMPTY,
	CARROT,
	CARROT_SEED,
	CAULIFLOWER,
	CAULIFLOWER_SEED,
	PUMPKIN,
	PUMPKIN_SEED,
	SUNFLOWER,
	SUNFLOWER_SEED,
	RADISH,
	RADISH_SEED,
	PARSNIP,
	PARSNIP_SEED,
	POTATO,
	POTATO_SEED,
	TURNIP,
	TURNIP_SEED,
	BEET,
	BEET_SEED,
	WHEAT,
	WHEAT_SEED,
	CABBAGE,
	CABBAGE_SEED
};

class ItemSlot {
private:
	int itemID;
	int quantity;
	int durability;
	sf::Text quantityText;

	static std::vector<Item> items;
	sf::RectangleShape slot;
	sf::RectangleShape itemIcon;
public:
	ItemSlot(sf::Vector2f position);
	static Item& getItem(int itemID);
	int getItemID() const;
	void setItem(int itemID);
	static void loadItems();

	int getQuantity() const;
	void setQuantity(int quantity);
	
	int getDurability() const;
	void setDurability(int durability);

	void drawSlot(sf::RenderWindow& window) const;
	void changeSlotOutlineColor(sf::Color color);
};
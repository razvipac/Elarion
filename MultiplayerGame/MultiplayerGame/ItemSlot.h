#pragma once
#include "Item.h"
#include <vector>
#include "SFML/Graphics.hpp"

enum ItemNames {
	EMPTY,
	CUP_EMPTY,
	CUP_WATER
};

class ItemSlot {
private:
	int itemID;
	int quantity;
	int durability;
	static std::vector<Item> items;
	sf::RectangleShape slot;
public:
	ItemSlot();
	void setItem(int itemID);
	void setQuantity(int quantity);
	void setDurability(int durability);
	int getItemID();
	int getQuantity();
	int getDurability();
	void drawSlot(sf::RenderWindow& window);
	static void loadItems();
	static Item getItem(int itemID);
};
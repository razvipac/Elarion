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
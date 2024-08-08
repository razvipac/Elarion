#pragma once

#include "ItemSlot.h"
#include "SFML/Graphics.hpp"
#include <vector>

class Inventory {
private:
	std::vector<ItemSlot> slots;
	sf::RectangleShape inventory;
	int selectedSlot;
public:
	Inventory();
	int getSelectedSlot();
	void setSelectedSlot(int slot);

	int getQuantity(int slot);
	void setQuantity(int slot, int quantity);

	void addItem(int itemID, int quantity);
	void changeItem(int slot, int itemID, int quantity);
	void removeItem(int itemID, int quantity);
	int getItemID(int slot);

	int getDurability(int slot);
	void setDurability(int slot, int durability);

	void drawInventory(sf::RenderWindow& window) const;
	void handleEvent(sf::Event event);
};
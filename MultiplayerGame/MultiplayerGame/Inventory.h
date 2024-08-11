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
	int getSelectedSlot() const;
	void setSelectedSlot(int slot);

	int getQuantity(int slot) const;
	void setQuantity(int slot, int quantity);

	void addItem(int itemID, int quantity);
	void changeItem(int slot, int itemID, int quantity);
	void removeItem(int itemID, int quantity);
	int getItemID(int slot) const;

	int getDurability(int slot) const;
	void setDurability(int slot, int durability);

	void drawInventory(sf::RenderWindow& window) const;
	void handleEvent(sf::Event& event);
};
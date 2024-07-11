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
	void setSelectedSlot(int selectedSlot);
	int getSelectedSlot();
	void addItem(int itemID, int quantity);
	void removeItem(int itemID, int quantity);
	void setQuantity(int slot, int quantity);
	int getQuantity(int slot);
	int getItemID(int slot);
	void setDurability(int slot, int durability);
	int getDurability(int slot);
	void setSelectedSlot(int slot);
	void drawInventory(sf::RenderWindow& window);
	//void loadItems();
};
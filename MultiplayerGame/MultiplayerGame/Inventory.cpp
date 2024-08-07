#include "Inventory.h"
using namespace std;
using namespace sf;

Inventory::Inventory() {
	selectedSlot = 0;
	inventory.setSize(Vector2f(500, 200));
	inventory.setFillColor(Color::White);
	inventory.setPosition(0, 0);

	for (int i = 0; i < 10; i++) {
		slots.push_back(ItemSlot(Vector2f(58 * i + 150, 500)));
	}
	slots[selectedSlot].changeSlotOutlineColor(Color::Green);

}
int Inventory::getSelectedSlot() {
	return selectedSlot;
}
void Inventory::addItem(int itemID, int quantity) {
	for (int i = 0; i < slots.size(); i++) {
		if (slots[i].getItemID() == itemID) {
			if (slots[i].getQuantity() + quantity <= ItemSlot::getItem(itemID).getMaxStackSize()) {
				slots[i].setQuantity(slots[i].getQuantity() + quantity);
				return;
			}
			else {
				quantity -= ItemSlot::getItem(itemID).getMaxStackSize() - slots[i].getQuantity();
				slots[i].setQuantity(ItemSlot::getItem(itemID).getMaxStackSize());
			}
		}
	}

	for (int i = 0; i < slots.size(); i++) {
		if (slots[i].getItemID() == 0) {
			slots[i].setItem(itemID);
			if (quantity <= ItemSlot::getItem(itemID).getMaxStackSize()) {

				slots[i].setQuantity(quantity);
				return;
			}
			else {
				slots[i].setQuantity(ItemSlot::getItem(itemID).getMaxStackSize());
				quantity -= ItemSlot::getItem(itemID).getMaxStackSize();
			}
		}
	}
	// de tratat cazul in care nu mai sunt sloturi disponibile
}
void Inventory::changeItem(int slot, int itemID, int quantity) {
	if (slot < 0 || slot >= slots.size())
		slot = 0;
	slots[slot].setItem(itemID);
	slots[slot].setQuantity(quantity);
}
void Inventory::removeItem(int itemID, int quantity) {
	for (int i = 0; i < slots.size(); i++) {
		if (slots[i].getItemID() == itemID) {
			if (slots[i].getQuantity() - quantity <= 0) {
				slots[i].setQuantity(0);
				slots[i].setDurability(0);
				slots[i].setItem(0);
				quantity -= slots[i].getQuantity();
			}
			else {
				slots[i].setQuantity(slots[i].getQuantity() - quantity);
				return;
			}
		}
	}

	// de tratat cazul in care nu exista suficiente iteme in inventar
}
void Inventory::setQuantity(int slot, int quantity) {
	if (slot < 0 || slot >= slots.size())
		slot = 0;
	slots[slot].setQuantity(quantity);
}
int Inventory::getQuantity(int slot) {
	if (slot < 0 || slot >= slots.size())
		slot = 0;
	return slots[slot].getQuantity();
}
int Inventory::getItemID(int slot) {
	if (slot < 0 || slot >= slots.size())
		slot = 0;
	return slots[slot].getItemID();
}
void Inventory::setDurability(int slot, int durability) {
	if (slot < 0 || slot >= slots.size())
		slot = 0;
	slots[slot].setDurability(durability);
}
int Inventory::getDurability(int slot) {
	if (slot < 0 || slot >= slots.size())
		slot = 0;
	return slots[slot].getDurability();
}
void Inventory::setSelectedSlot(int slot) {
	if (slot < 0 || slot >= 10)
		slot = 0;
	slots[selectedSlot].changeSlotOutlineColor(Color::Black);
	selectedSlot = slot;
	slots[selectedSlot].changeSlotOutlineColor(Color::Green);
}
void Inventory::drawInventory(RenderWindow& window) {
	//window.draw(inventory);
	for (int i = 0; i < slots.size(); i++) {
		slots[i].drawSlot(window);
	}
}
void Inventory::handleEvent(Event event) {
	if (event.type == Event::KeyPressed) {
		if (event.key.code == Keyboard::Num1)
			setSelectedSlot(0);
		if (event.key.code == Keyboard::Num2)
			setSelectedSlot(1);
		if (event.key.code == Keyboard::Num3)
			setSelectedSlot(2);
		if (event.key.code == Keyboard::Num4)
			setSelectedSlot(3);
		if (event.key.code == Keyboard::Num5)
			setSelectedSlot(4);
		if (event.key.code == Keyboard::Num6)
			setSelectedSlot(5);
		if (event.key.code == Keyboard::Num7)
			setSelectedSlot(6);
		if (event.key.code == Keyboard::Num8)
			setSelectedSlot(7);
		if (event.key.code == Keyboard::Num9)
			setSelectedSlot(8);
		if (event.key.code == Keyboard::Num0)
			setSelectedSlot(9);

		// if we pressed t change the selected item to an empty cup
		if (event.key.code == Keyboard::T)
			changeItem(selectedSlot, 1, 1);
		// if we pressed y change the selected item to a full cup
		if (event.key.code == Keyboard::Y)
			changeItem(selectedSlot, 2, 1);
	}
	if (event.type == Event::MouseWheelScrolled) {
		if (event.mouseWheelScroll.delta > 0) {
			if (selectedSlot == 0)
				setSelectedSlot(9);
			else
				setSelectedSlot(selectedSlot - 1);
		}
		else {
			if (selectedSlot == 9)
				setSelectedSlot(0);
			else
				setSelectedSlot(selectedSlot + 1);
		}
	}
}


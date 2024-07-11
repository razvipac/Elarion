#pragma once
class Interactable {
	public:
	virtual void interact(int itemId) = 0;
	virtual void hover(int itemId) = 0;
};
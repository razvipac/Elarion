#include <cstring>
#include "Utility.h"

void packMovementData(char* buffer, char c, int i, float f1, float f2) {
	// Ensure buffer is large enough: 1 byte for char, 4 bytes for int, 4 bytes each for 2 floats
	// Total = 1 + 4 + 4 + 4 = 13 bytes

	// Copy the char
	memcpy(buffer, &c, sizeof(char));

	// Copy the int, starting at offset 1
	memcpy(buffer + 1, &i, sizeof(int));

	// Copy the first float, starting at offset 5 (1 + 4)
	memcpy(buffer + 1 + sizeof(int), &f1, sizeof(float));

	// Copy the second float, starting at offset 9 (1 + 4 + 4)
	memcpy(buffer + 1 + sizeof(int) + sizeof(float), &f2, sizeof(float));
}
void unpackMovementData(const char* buffer, char& c, int& i, float& f1, float& f2) {
	// Extract the char
	memcpy(&c, buffer, sizeof(char));

	// Extract the int, starting at offset 1
	memcpy(&i, buffer + 1, sizeof(int));

	// Extract the first float, starting at offset 5 (1 + 4)
	memcpy(&f1, buffer + 1 + sizeof(int), sizeof(float));

	// Extract the second float, starting at offset 9 (1 + 4 + 4)
	memcpy(&f2, buffer + 1 + sizeof(int) + sizeof(float), sizeof(float));
}

void packHitData(char* buffer, char type, int currentId, int targetId, float damage) {
	// Ensure buffer is large enough: 1 byte for char, 4 bytes for int, 4 bytes each for 2 floats
	// Total = 1 + 4 + 4 + 4 = 13 bytes

	// Copy the char
	memcpy(buffer, &type, sizeof(char));

	// Copy the int, starting at offset 1
	memcpy(buffer + 1, &currentId, sizeof(int));

	// Copy the first int, starting at offset 5 (1 + 4)
	memcpy(buffer + 1 + sizeof(int), &targetId, sizeof(int));

	// Copy the first float, starting at offset 9 (1 + 4 + 4)
	memcpy(buffer + 1 + sizeof(int) + sizeof(int), &damage, sizeof(float));
}
void unpackHitData(const char* buffer, char& type, int& currentId, int& targetId, float& damage) {
	// Extract the char
	memcpy(&type, buffer, sizeof(char));

	// Extract the int, starting at offset 1
	memcpy(&currentId, buffer + 1, sizeof(int));

	// Extract the first int, starting at offset 5 (1 + 4)
	memcpy(&targetId, buffer + 1 + sizeof(int), sizeof(int));

	// Extract the first float, starting at offset 9 (1 + 4 + 4)
	memcpy(&damage, buffer + 1 + sizeof(int) + sizeof(int), sizeof(float));
}
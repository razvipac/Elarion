#pragma once

void unpackMovementData(const char* buffer, char& c, int& i, float& f1, float& f2);
void packMovementData(char* buffer, char c, int i, float f1, float f2);

void unpackHitData(const char* buffer, char& type, int& currentId, int& targetId, float& damage);
void packHitData(char* buffer, char type, int currentId, int targetId, float damage);
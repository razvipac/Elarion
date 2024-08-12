#pragma once
#include "SFML/Graphics.hpp"
#include "enet/enet.h"
#include <iostream>

class NetworkManager
{
private:
	ENetAddress address;
	ENetHost* client;
	ENetPeer* peer;
	ENetEvent event;
	std::string serverIP;
	std::string localIP;
	int serverPort;
	bool local;
	NetworkManager();
public:
	static NetworkManager& getInstance();

	NetworkManager(const NetworkManager&) = delete;
	NetworkManager& operator=(const NetworkManager&) = delete;

	void sendPacket(const char* data, int packetSize, bool reliable = true);
	void parseData(char* data, int dataSize);

	void msgLoop();

	~NetworkManager();
};
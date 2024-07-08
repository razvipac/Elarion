#include <iostream>
#include "Client.h"
#include "enet/enet.h"
#include <thread>
#include <string>
#include <chrono>
#include "ClientData.h"
#include <map>

using namespace std;

bool isRunning = true;
int CLIENTID = -1;
map<int, ClientData*> clientMap;

void sendPacket(ENetPeer* peer, const char* data) {
	ENetPacket* packet = enet_packet_create(data, strlen(data) + 1, ENET_PACKET_FLAG_RELIABLE);
	enet_peer_send(peer, 0, packet);
}

void parseData(char* data) {
	int dataType;
	int id;
	sscanf_s(data, "%d|%d", &dataType, &id);

	switch (dataType) {
	case 1: {
		if (id != CLIENTID) {
			char msg[80];
			sscanf_s(data, "%*d|%*d|%[^|]", &msg);
			cout << clientMap[id]->getUsername().c_str() << ": " << msg << "\n";
		}
		break;
	}
	case 2:
	{
		if (id != CLIENTID) {
			char username[80];
			sscanf_s(data, "%*d|%*d|%[^|]", &username);

			clientMap[id] = new ClientData(id);
			clientMap[id]->setUsername(username);
		}
		break;
	}
	case 3:
		CLIENTID = id;
		break;
	}
}

void msgLoop(ENetHost* client) {
	while (isRunning) {
		ENetEvent event;
		while (enet_host_service(client, &event, 0) > 0) {
			if (event.type == ENET_EVENT_TYPE_RECEIVE) {
				parseData((char*) event.packet->data);
				enet_packet_destroy(event.packet);
			}
		}
	}
}

int clientMain() {
	
	if (enet_initialize()) {
		cout << "An error occured while initializing ENet!\n";
		return EXIT_FAILURE;
	}
	atexit(enet_deinitialize);

	ENetHost* client;
	client = enet_host_create(NULL, 1, 1, 0, 0);

	if (!client) {
		cout << "An error occured while trying to create an ENet Client host";
		return EXIT_FAILURE;
	}

	ENetAddress address;
	ENetEvent event;
	ENetPeer* peer;

	enet_address_set_host(&address, "127.0.0.1"); // 172.205.150.168
	address.port = 8080;
	peer = enet_host_connect(client, &address, 1, 0);
	
	if (!peer) {
		cout << "No available peers for initiating an ENet connection!";
		return EXIT_FAILURE;
	}

	if (enet_host_service(client, &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT)
		cout << "Connection to 127.0.0.1:8080 succeeded.";
	else {
		enet_peer_reset(peer);
		cout << "Connection to 127.0.0.1:8080 failed.";
		return EXIT_SUCCESS;
	}

	string name;
	cout << "Enter name: ";
	cin >> name;
	cin.get();

	string info = "2|" + name;
	sendPacket(peer, info.c_str());

	// PROGRAM LOOP START

	thread t{msgLoop, client};

	string str;
	while (isRunning) {
		getline(cin, str);
		if (str == "exit") {
			isRunning = false;
			break;
		}
		cout << name << ": " << str << "\n";
		str = "1|" + str;
		sendPacket(peer, str.c_str());
	}

	// PROGRAM LOOP END

	t.join();

	enet_peer_disconnect(peer, 0);

	while (enet_host_service(client, &event, 3000) > 0) {
		if (event.type == ENET_EVENT_TYPE_RECEIVE)
			enet_packet_destroy(event.packet);
		else if (event.type == ENET_EVENT_TYPE_DISCONNECT)
			cout << "Disconnection succeeded.\n";
	}

	return EXIT_SUCCESS;
}
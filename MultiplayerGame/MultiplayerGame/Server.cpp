#include <iostream>
#include "Server.h"
#include "enet/enet.h"
#include <map>
#include "ClientData.h"

using namespace std;

map<int, ClientData*> ClientMap;

void broadcastPacket(ENetHost* server, const char* data) {
	ENetPacket* packet = enet_packet_create(data, strlen(data) + 1, ENET_PACKET_FLAG_RELIABLE);
	enet_host_broadcast(server, 0, packet);
}

void SendPacket(ENetPeer* peer, const char* data) {
	ENetPacket* packet = enet_packet_create(data, strlen(data) + 1, ENET_PACKET_FLAG_RELIABLE);
	enet_peer_send(peer, 0, packet);
}

void parseData(ENetHost* server, int id, char* data) {
	cout << "Parse: " << data << "\n";

	// to initialise dataType with -1
	int dataType;
	sscanf_s(data, "%d|", &dataType);
	cout << "Asta e dataType: " << dataType << "\n";
	switch (dataType) {
	case 1: {
		char msg[80];
		sscanf_s(data, "%*d|%[^\n]", &msg);

		char sendData[1024] = { '\0' };
		sprintf_s(sendData, "1|%d|%s", id, msg);
		broadcastPacket(server, sendData);
		break;
	}
	case 2: {
		char name[80];
		sscanf_s(data, "2|%[^\n]", &name);

		char sendData[1024] = { '\0' };
		sprintf_s(sendData, "2|%d|%s", id, name);

		broadcastPacket(server, sendData);
		ClientMap[id]->setUsername(name);
		break;
	}
	}
}

int serverMain() {

	if (enet_initialize()) {
		cout << "An error occurred while initializing ENet.\n";
		return EXIT_FAILURE;
	}
	atexit(enet_deinitialize);

	ENetAddress address;
	ENetHost* server;
	ENetEvent event;

	address.host = ENET_HOST_ANY;
	address.port = 8080;
	server = enet_host_create(&address, 32, 1, 0, 0);

	if (!server) {
		cout << "An error occurred while trying to create an ENet server host!\n";
		return EXIT_FAILURE;
	}

	//// PROGRAM LOOP START
	int newPlayerId = 0;


	while (true) {
		while (enet_host_service(server, &event, 1000) > 0) {
			if (event.type == ENET_EVENT_TYPE_CONNECT) {
				cout << "A new client connected from " << event.peer->address.host
					<< ":" << event.peer->address.port;
				
				for (auto const& x : ClientMap) {
					char sendData[1024] = { '\0' };
					sprintf_s(sendData, "2|%d|%s", x.first, x.second -> getUsername().c_str());
					broadcastPacket(server, sendData);
				}

				newPlayerId++;
				ClientMap[newPlayerId] = new ClientData(newPlayerId);
				event.peer->data = ClientMap[newPlayerId];

				char dataToSend[126] = { '\0' };
				sprintf_s(dataToSend, "3|%d", newPlayerId);
				SendPacket(event.peer, dataToSend);

			}
			else if (event.type == ENET_EVENT_TYPE_RECEIVE) {
				cout << "A packet of length " << event.packet->dataLength
					<< "containing " << event.packet->data
					<< "was received from " << event.peer->data
					<< "on IP " << event.peer->address.host
					<< ":" << event.peer->address.port
					<< "on channel " << event.channelID << "\n";
				parseData(server, static_cast<ClientData*>(event.peer->data)->getId(), (char*)event.packet->data);
				enet_packet_destroy(event.packet);
			}
			else if (event.type == ENET_EVENT_TYPE_DISCONNECT) {
				cout << event.peer->address.host << event.peer->address.port << " disconnected.\n";
				char disconnectedData[126] = { '\0' };
				sprintf_s(disconnectedData, "4|%d", static_cast<ClientData*>(event.peer->data)->getId());
				broadcastPacket(server, disconnectedData);

				event.peer->data = NULL;
			}
		}
	}

	//// PROGRAM LOOP END

	enet_host_destroy(server);

	return EXIT_SUCCESS;
}
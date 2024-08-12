#include <iostream>
#include "enet/enet.h"
#include <map>
#include "Utility.h"

using namespace std;

map<int, int*> clientsMap;

void broadcastPacket(ENetHost* server, const char* data, int dataSize, bool reliable = true) {
	ENetPacket* packet;
	if (reliable)
		packet = enet_packet_create(data, dataSize, ENET_PACKET_FLAG_RELIABLE);
	else
		packet = enet_packet_create(data, dataSize, 0);
	enet_host_broadcast(server, 0, packet);
}

void SendPacket(ENetPeer* peer, const char* data, int dataSize, bool reliable = true) {
	ENetPacket* packet;
	if (reliable)
		packet = enet_packet_create(data, dataSize, ENET_PACKET_FLAG_RELIABLE);
	else
		packet = enet_packet_create(data, dataSize, 0);
	enet_peer_send(peer, 0, packet);
}

void parseData(ENetHost* server, int id, char* data, int dataSize) {
	cout << "Parse: " << data << "\n";

	char dataType;
	dataType = data[0];
	cout << "Asta e dataType: " << dataType << "\n";

	switch (dataType) {
		case 1:
		case 2:
		case 5:
			broadcastPacket(server, data, dataSize);
			break;
	}
}

int main() {

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
					<< ":" << event.peer->address.port << "\n";

				/*for (auto const& x : ClientMap) {
					char sendData[1024] = { '\0' };
					sprintf_s(sendData, "2|%d|%s", x.first, x.second -> getUsername().c_str());
					broadcastPacket(server, sendData);
				}*/

				for (auto const& x : clientsMap) {
					char message[13];
					packMovementData(message, 2, x.first, 50, 20);
					/*int id;
					float x1, y;
					char c;
					unpackMovementData(message, c, id, x1, y);
					cout << "Message: " << message << "\n";
					cout << "x.first: " << x.first << "\n";
					cout << "id: " << id << "\n";
					cout << "x1: " << x1 << "\n";
					cout << "y: " << y << "\n";*/

					/*for (int i = 0; i < 13; i++)
						cout << message[i] << " ";
					cout << "\n";*/

					SendPacket(event.peer, message, 13);
				}

				newPlayerId++;
				clientsMap[newPlayerId] = new int(newPlayerId);
				event.peer->data = clientsMap[newPlayerId];

				char message[13];
				packMovementData(message, 3, newPlayerId, 0, 0);
				SendPacket(event.peer, message, 13);

				packMovementData(message, 2, newPlayerId, 50, 20);
				broadcastPacket(server, message, 13);

			}
			else if (event.type == ENET_EVENT_TYPE_RECEIVE) {
				cout << "A packet of length " << event.packet->dataLength
					<< "containing " << event.packet->data
					<< "was received from " << event.peer->data
					<< "on IP " << event.peer->address.host
					<< ":" << event.peer->address.port
					<< "on channel " << event.channelID << "\n";
				parseData(server, *((int*)event.peer->data), (char*)event.packet->data, event.packet->dataLength);
				enet_packet_destroy(event.packet);
			}
			else if (event.type == ENET_EVENT_TYPE_DISCONNECT) {
				cout << event.peer->address.host << event.peer->address.port << " disconnected.\n";
				char message[13];
				int id = *((int*)event.peer->data);
				packMovementData(message, 4, id, 0, 0);
				//char disconnectedData[126] = { '\0' };
				//sprintf_s(disconnectedData, "4|%d", *((int*)event.peer->data));
				broadcastPacket(server, message, 13);
				clientsMap.erase(id);
				event.peer->data = NULL;
			}
		}
	}

	//// PROGRAM LOOP END

	enet_host_destroy(server);

	return EXIT_SUCCESS;
}
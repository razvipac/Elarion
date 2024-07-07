#include <iostream>
#include "Server.h"
#include "enet/enet.h"

using namespace std;

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

	while (true) {
		while (enet_host_service(server, &event, 1000) > 0) {
			if (event.type == ENET_EVENT_TYPE_CONNECT)
				cout << "A new client connected from " << event.peer->address.host
				<< ":" << event.peer->address.port;
			else if (event.type == ENET_EVENT_TYPE_RECEIVE)
				cout << "A packet of length " << event.packet->dataLength
				<< "containing " << event.packet->data
				<< "was received from " << event.peer->data
				<< "on IP " << event.peer->address.host
				<< ":" << event.peer->address.port
				<< "on channel " << event.channelID << "\n";
			else if (event.type == ENET_EVENT_TYPE_DISCONNECT)
				cout << event.peer->address.host << event.peer->address.port << " disconnected.\n";
		}
	}

	//// PROGRAM LOOP END

	enet_host_destroy(server);

	return EXIT_SUCCESS;
}
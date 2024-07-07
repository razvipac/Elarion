#include <iostream>
#include "Client.h"
#include "enet/enet.h"

using namespace std;

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

	enet_address_set_host(&address, "127.0.0.1");
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

	// PROGRAM LOOP START

	while (enet_host_service(client, &event, 1000) > 0) {
		if (event.type == ENET_EVENT_TYPE_RECEIVE) {
			cout << "A packet of length " << event.packet->dataLength
				 << "containing "         << event.packet->data
				 << "was received from "  << event.peer->data
				 << "on IP "              << event.peer->address.host
				 << ":"                   << event.peer->address.port
				 << "on channel "         << event.channelID<< "\n";
		}
	}

	// PROGRAM LOOP END

	enet_peer_disconnect(peer, 0);

	while (enet_host_service(client, &event, 3000) > 0) {
		if (event.type == ENET_EVENT_TYPE_RECEIVE)
			enet_packet_destroy(event.packet);
		else if (event.type == ENET_EVENT_TYPE_DISCONNECT)
			cout << "Disconnection succeeded.\n";
	}

	return EXIT_SUCCESS;
}
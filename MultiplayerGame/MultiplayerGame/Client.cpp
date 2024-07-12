#include <iostream>
#include "Client.h"
#include "enet/enet.h"
#include <string>
#include "Player.h"
#include <cstring>
#include <map>
#include "Utility.h"
#include "ItemSlot.h"
#include "Tile.h"

using namespace std;
using namespace sf;

//bool isRunning = true;
//map<int, ClientData*> clientMap;
map<int, Player*> playerMap;
Player* playerPointer;
int CLIENTID = -1;

void sendPacket(ENetPeer* peer, const char* data, bool reliable = true) {
	ENetPacket* packet;
	if(reliable)
		packet = enet_packet_create(data, 13, ENET_PACKET_FLAG_RELIABLE);
	else
		packet = enet_packet_create(data, 13, 0);
	enet_peer_send(peer, 0, packet);
}

float charArrayToFloat(const char* data) {
	float result;
	memcpy(&result, data, sizeof(float)); // Copy the first 4 bytes into result
	return result;
}

void parseData(char* data) {

	if(strlen(data) < 1)
		return;
	char messageType = data[0];
	cout << "Message type: " << messageType << "\n";
	if (messageType == '1') {
		int id;
		float x, y;
		unpackData(data, messageType, id, x, y);
		cout << "ID: " << id << " X: " << x << " Y: " << y << "\n";
		if (id != CLIENTID && playerMap.find(id) != playerMap.end()) {
			playerMap[id]->setTargetPosition(Vector2f(x, y));
		}
	}
	else if (messageType == '2') {
		int id;
		float x, y;
		unpackData(data, messageType, id, x, y);
		for (int i = 0; i < 13; i++)
			cout << data << " ";
		cout << "\n";
		cout << "ID: " << id << " X: " << x << " Y: " << y << "\n";
		if (id != CLIENTID) {
			playerMap[id] = new Player(id);
			playerMap[id]->setPosition(Vector2f(x, y));
		}
	}
	else if (messageType == '3') {
		int id;
		float x, y;
		unpackData(data, messageType, id, x, y);
		CLIENTID = id;
		if (playerPointer) {
			playerPointer->setId(id);
			cout << "am intrat" << "\n";
		}
		cout << "Asta e CLIENTID: " << CLIENTID << "\n";
		cout << "ID: " << id << " X: " << x << " Y: " << y << "\n";
	}
	else if (messageType == '4') {
		int id;
		float x, y;
		unpackData(data, messageType, id, x, y);
		cout << "ID: " << id << " X: " << x << " Y: " << y << "\n";
		if (id != CLIENTID) {
			delete playerMap[id];
			playerMap.erase(id);
		}
	}
	else {
		cout << "Unknown message type: " << messageType << "\n";
	}
	

	//int dataType;
	//int id;
	//sscanf_s(data, "%d|%d", &dataType, &id);
	////dataType = stoi("" + data[0]);

	//switch (dataType) {
	//case 1: {
	//	if (id != CLIENTID) {
	//		char msg[80];
	//		sscanf_s(data, "%*d|%*d|%[^|]", &msg);
	//		cout << clientMap[id]->getUsername().c_str() << ": " << msg << "\n";
	//	}
	//	break;
	//}
	//case 2:
	//{
	//	if (id != CLIENTID) {
	//		char username[80];
	//		sscanf_s(data, "%*d|%*d|%[^|]", &username);

	//		clientMap[id] = new ClientData(id);
	//		clientMap[id]->setUsername(username);
	//	}
	//	break;
	//}
	//case 3:
	//	CLIENTID = id;
	//	break;
	//}
}

void msgLoop(ENetHost* client) {
	ENetEvent event;
	while (enet_host_service(client, &event, 0) > 0) {
		if (event.type == ENET_EVENT_TYPE_RECEIVE) {
			parseData((char*)event.packet->data);
			enet_packet_destroy(event.packet);
		}
		else if (event.type == ENET_EVENT_TYPE_DISCONNECT)
			cout << "Disconnection succeeded.\n";
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

	// 127.0.0.1:8080
	enet_address_set_host(&address, "127.0.0.1"); // 172.205.150.168
	address.port = 8080;
	peer = enet_host_connect(client, &address, 1, 0);
	
	if (!peer) {
		cout << "No available peers for initiating an ENet connection!";
		return EXIT_FAILURE;
	}

	if (enet_host_service(client, &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT)
		cout << "Connection to " << address.host << ":" << address.port << " succeeded.";
	else {
		enet_peer_reset(peer);
		cout << "Connection to " << address.host << ":" << address.port << " failed.";
		return EXIT_SUCCESS;
	}


	/////////////////////////////////////////////////



	ItemSlot::loadItems();

	RenderWindow window(VideoMode(800, 600), "Client");
	Player player(CLIENTID);
	playerPointer = &player;
	cout << CLIENTID << "\n";
	
	Tile waterTile(1, true, "Resources/Textures/WaterTile.png");
	RectangleShape waterTileShape(Vector2f(50, 50));
	waterTileShape.setTexture(waterTile.getTexture());
	waterTileShape.setPosition(Vector2f(100, 100));

	Clock deltaClock;
	Time currentTime = deltaClock.getElapsedTime();
	float packageTimeCounter = 0.0f;

	while (window.isOpen()) {
		currentTime = deltaClock.restart();
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed)
				window.close();
			player.handleEvent(event);
			for(auto& p : playerMap) {
				p.second->handleEvent(event);
			}
			if(event.type == Event::MouseButtonPressed) {
				if(event.mouseButton.button == Mouse::Left) {
					Vector2i mousePos = Mouse::getPosition(window);
					Vector2f worldPos = window.mapPixelToCoords(mousePos);
					if(waterTileShape.getGlobalBounds().contains(worldPos) && player.getSelectedItemId() == ItemNames::CUP_EMPTY) {
						player.setSelectedItemId(ItemNames::CUP_WATER);
					}
				}
			}
		}

		msgLoop(client);

		player.update(currentTime.asSeconds());
		for(auto& p : playerMap) {
			p.second->update(currentTime.asSeconds());
		}

		packageTimeCounter += currentTime.asSeconds();
		if(packageTimeCounter >= 0.1f) {
			char message[13];
			packData(message, '1', CLIENTID, player.getPosition().x, player.getPosition().y);
			sendPacket(peer, message);
			packageTimeCounter = 0.0f;
		}

		window.clear();
		window.draw(waterTileShape);
		player.drawPlayer(window);
		for(auto& p : playerMap) {
			p.second->drawPlayer(window);
		}
		window.display();
	}

	cout << "hatz";
	enet_peer_disconnect(peer, 0);
	cout << "dorian";


	while (enet_host_service(client, &event, 3000) > 0) {
		if (event.type == ENET_EVENT_TYPE_RECEIVE)
			enet_packet_destroy(event.packet);
		else if (event.type == ENET_EVENT_TYPE_DISCONNECT)
			cout << "Disconnection succeeded.\n";
	}
	cout << "popa";

	return EXIT_SUCCESS;
	
	
	
	
	
	
	
	//if (enet_initialize()) {
	//	cout << "An error occured while initializing ENet!\n";
	//	return EXIT_FAILURE;
	//}
	//atexit(enet_deinitialize);

	//ENetHost* client;
	//client = enet_host_create(NULL, 1, 1, 0, 0);

	//if (!client) {
	//	cout << "An error occured while trying to create an ENet Client host";
	//	return EXIT_FAILURE;
	//}

	//ENetAddress address;
	//ENetEvent event;
	//ENetPeer* peer;

	//enet_address_set_host(&address, "172.205.150.168"); // 172.205.150.168
	//address.port = 8080;
	//peer = enet_host_connect(client, &address, 1, 0);
	//
	//if (!peer) {
	//	cout << "No available peers for initiating an ENet connection!";
	//	return EXIT_FAILURE;
	//}

	//if (enet_host_service(client, &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT)
	//	cout << "Connection to 127.0.0.1:8080 succeeded.";
	//else {
	//	enet_peer_reset(peer);
	//	cout << "Connection to 127.0.0.1:8080 failed.";
	//	return EXIT_SUCCESS;
	//}

	//string name;
	//cout << "Enter name: ";
	//cin >> name;
	//cin.get();

	//string info = "2|" + name;
	//sendPacket(peer, info.c_str());

	//// PROGRAM LOOP START


	//string str;
	//while (isRunning) {
	//	getline(cin, str);
	//	if (str == "/exit") {
	//		isRunning = false;
	//		break;
	//	}
	//	cout << name << ": " << str << "\n";
	//	str = "1|" + str;
	//	sendPacket(peer, str.c_str());
	//}

	//// PROGRAM LOOP END

	/*enet_peer_disconnect(peer, 0);

	while (enet_host_service(client, &event, 3000) > 0) {
		if (event.type == ENET_EVENT_TYPE_RECEIVE)
			enet_packet_destroy(event.packet);
		else if (event.type == ENET_EVENT_TYPE_DISCONNECT)
			cout << "Disconnection succeeded.\n";
	}

	return EXIT_SUCCESS;*/
}
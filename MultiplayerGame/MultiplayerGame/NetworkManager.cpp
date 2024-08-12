#include "NetworkManager.h"
#include "Utility.h"
#include <map>
#include "Player.h"

using namespace std;
using namespace sf;

extern int CLIENTID;
extern map<int, Player*> playerMap;
extern Player* playerPointer;

NetworkManager::NetworkManager()
{
	local = true;


	serverIP = "172.205.150.168";
	localIP = "127.0.0.1";
	serverPort = 8080;

	if (enet_initialize() != 0)
	{
		cout << "An error occurred while initializing ENet." << endl;
		exit(EXIT_FAILURE);
	}

	client = enet_host_create(NULL, 1, 1, 0, 0);

	if (client == NULL)
	{
		cout << "An error occurred while trying to create an ENet client host." << endl;
		exit(EXIT_FAILURE);
	}

	enet_address_set_host(&address, local ? localIP.c_str() : serverIP.c_str());
	address.port = serverPort;

	peer = enet_host_connect(client, &address, 1, 0);

	if (peer == NULL)
	{
		cout << "No available peers for initiating an ENet connection." << endl;
		exit(EXIT_FAILURE);
	}

	if (enet_host_service(client, &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT)
	{
		cout << "Connection to " << (local ? localIP : serverIP) << " succeeded." << endl;
	}
	else
	{
		enet_peer_reset(peer);
		cout << "Connection to " << (local ? localIP : serverIP) << " failed." << endl;
		exit(EXIT_FAILURE);
	}
}

NetworkManager& NetworkManager::getInstance()
{
	static NetworkManager instance;
	return instance;
}

void NetworkManager::sendPacket(const char* data, int packetSize, bool reliable)
{
	ENetPacket* packet = enet_packet_create(data, packetSize, reliable ? ENET_PACKET_FLAG_RELIABLE : 0);
	enet_peer_send(peer, 0, packet);
}

void NetworkManager::parseData(char* data, int dataSize)
{
	if (dataSize < 1) // STRLEN poate cauza probleme
		return;

	char messageType = data[0];
	//cout << "Message type: " << messageType << "\n";

	if (messageType == 1) { // Movement for the other players
		int id;
		float x, y;
		unpackMovementData(data, messageType, id, x, y);
		//cout << "ID: " << id << " X: " << x << " Y: " << y << "\n";
		if (id != CLIENTID && playerMap.find(id) != playerMap.end()) {
			playerMap[id]->setTargetPosition(Vector2f(x, y));
			//cout << "Moving the player\n";
		}
	}

	else if (messageType == 2) { // New player
		int id;
		float x, y;
		unpackMovementData(data, messageType, id, x, y);
		for (int i = 0; i < 13; i++)
			cout << data << " ";
		//cout << "\n";
		//cout << "ID: " << id << " X: " << x << " Y: " << y << "\n";
		if (id != CLIENTID) {
			playerMap[id] = new Player(id);
			playerMap[id]->setPosition(Vector2f(x, y));
		}
	}

	else if (messageType == 3) { // Receiving the client's ID from the server
		int id;
		float x, y;
		unpackMovementData(data, messageType, id, x, y);
		CLIENTID = id;
		if (playerPointer) {
			playerPointer->setId(id);
			cout << "am intrat" << "\n";
		}
		cout << "Asta e CLIENTID: " << CLIENTID << "\n";
		//cout << "ID: " << id << " X: " << x << " Y: " << y << "\n";
	}

	else if (messageType == 4) { // Player disconnection
		int id;
		float x, y;
		unpackMovementData(data, messageType, id, x, y);
		//cout << "ID: " << id << " X: " << x << " Y: " << y << "\n";
		if (id != CLIENTID) {
			delete playerMap[id];
			playerMap.erase(id);
		}
	}

	else if (messageType == 5) { // Entity hit
		int currentId, targetId;
		float damage;
		unpackHitData(data, messageType, currentId, targetId, damage);
		cout<<"Current ID: "<<currentId<<" Target ID: "<<targetId<<" Damage: "<<damage<<endl;
		if (playerMap.find(targetId) != playerMap.end() && currentId != CLIENTID) {
			cout << "Player " << targetId << " took " << damage << " damage from player " << currentId << "\n";
			playerMap[targetId]->takeDamage(damage);
		}
		if(playerMap.find(currentId) != playerMap.end() && currentId != CLIENTID)
			playerMap[currentId]->attackVisual();
	}

	else {
		cout << "Unknown message type: " << messageType << "\n";
	}
}

void NetworkManager::msgLoop() {
	while (enet_host_service(client, &event, 0) > 0) {
		if (event.type == ENET_EVENT_TYPE_RECEIVE) {
			parseData((char*)event.packet->data, event.packet->dataLength);
			enet_packet_destroy(event.packet);
		}
		else if (event.type == ENET_EVENT_TYPE_DISCONNECT)
			cout << "Disconnection succeeded.\n";
	}
}

NetworkManager::~NetworkManager()
{
	enet_peer_disconnect(peer, 0);
	while (enet_host_service(client, &event, 3000) > 0)
	{
		switch (event.type)
		{
		case ENET_EVENT_TYPE_RECEIVE:
			enet_packet_destroy(event.packet);
			break;
		case ENET_EVENT_TYPE_DISCONNECT:
			cout << "Disconnection succeeded.\n";
			break;
		}
	}
	enet_peer_reset(peer);
	enet_host_destroy(client);
	enet_deinitialize();
}
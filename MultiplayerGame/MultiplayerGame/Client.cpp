#include "SFML/Graphics.hpp" //LINIA ASTA SA FIE PESTE ENET SAU CRAPA!!!!!!!!!!!!
#include <iostream>
#include "enet/enet.h"
#include <string>
#include "Player.h"
#include <cstring>
#include <map>
#include "Utility.h"
#include "ItemSlot.h"
#include "Tile.h"
#include "PerlinNoise.h"
#include "TileMap.h"
#include "TextureManager.h"
#include "Animator.h"

using namespace std;
using namespace sf;

map<int, Player*> playerMap;
Player* playerPointer;
int CLIENTID = -1;
int tps = 10;

void sendPacket(ENetPeer* peer, const char* data, bool reliable = true) {
	ENetPacket* packet;
	if (reliable)
		packet = enet_packet_create(data, 13, ENET_PACKET_FLAG_RELIABLE);
	else
		packet = enet_packet_create(data, 13, 0);
	enet_peer_send(peer, 0, packet);
}

void parseData(char* data) {

	if (strlen(data) < 1) // STRLEN poate cauza probleme
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
			cout << "Moving the player\n";
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

int main() {
	cout << "Starting\n";
	srand(time(NULL));

	TextureManager::getInstance().loadTexture("PlayerIdle", "Resources/Sunnyside_World_Assets/Characters/Human/IDLE/base_idle_strip9.png");
	TextureManager::getInstance().loadTexture("PlayerWalk", "Resources/Sunnyside_World_Assets/Characters/Human/WALKING/base_walk_strip8.png");
	TextureManager::getInstance().loadTexture("PlayerRun", "Resources/Sunnyside_World_Assets/Characters/Human/RUN/base_run_strip8.png");

	PerlinNoise noise;
	noise.saveNoise2DColored("Resources/Textures/PerlinNoise.png");

	//RenderWindow window2(VideoMode(1024, 1024), "Noise");

	Vector2f lastMousePos;

	const int width = 256;
	const int height = 256;
	int tileSize = 62;
	int level[width * height];
	noise.get2DNoiseColored(level);
	TileMap map;
	if (!map.load("Resources/Textures/TileSet.png", Vector2u(tileSize, tileSize), level, width, height)) {
		cout << "Failed to load map\n";
		return EXIT_FAILURE;
	}

	/*cout << "Loaded everything\n";

	Clock deltaClock;
	Time currentTime = deltaClock.getElapsedTime();


	while (window2.isOpen()) {
		currentTime = deltaClock.restart();
		Event event;
		while (window2.pollEvent(event)) {
			if (event.type == Event::Closed)
				window2.close();

			if (event.type == Event::KeyPressed) {
				if (event.key.code == Keyboard::Space) {
					noise.changePerlinNoise2DSeed();
					noise.saveNoise2DColored("Resources/Textures/PerlinNoise.png");
					noise.get2DNoiseColored(level);
					map.update(level, Vector2u(tileSize, tileSize), width, height);
					//noiseTexture.loadFromFile("Resources/Textures/PerlinNoise.png");
					//noiseSprite.setTexture(noiseTexture);
				}
				else if (event.key.code == Keyboard::Up) {
					noise.increaseOctaves();
					noise.saveNoise2DColored("Resources/Textures/PerlinNoise.png");
					noise.get2DNoiseColored(level);
					map.update(level, Vector2u(tileSize, tileSize), width, height);
					//noiseTexture.loadFromFile("Resources/Textures/PerlinNoise.png");
					//noiseSprite.setTexture(noiseTexture);
				}
				else if (event.key.code == Keyboard::Down) {
					noise.decreaseOctaves();
					noise.saveNoise2DColored("Resources/Textures/PerlinNoise.png");
					noise.get2DNoiseColored(level);
					map.update(level, Vector2u(tileSize, tileSize), width, height);
					//noiseTexture.loadFromFile("Resources/Textures/PerlinNoise.png");
					//noiseSprite.setTexture(noiseTexture);
				}
				else if (event.key.code == Keyboard::B) {
					noise.increaseBias();
					noise.saveNoise2DColored("Resources/Textures/PerlinNoise.png");
					noise.get2DNoiseColored(level);
					map.update(level, Vector2u(tileSize, tileSize), width, height);
					//noiseTexture.loadFromFile("Resources/Textures/PerlinNoise.png");
					//noiseSprite.setTexture(noiseTexture);
				}
				else if (event.key.code == Keyboard::N) {
					noise.decreaseBias();
					noise.saveNoise2DColored("Resources/Textures/PerlinNoise.png");
					noise.get2DNoiseColored(level);
					map.update(level, Vector2u(tileSize, tileSize), width, height);
					//noiseTexture.loadFromFile("Resources/Textures/PerlinNoise.png");
					//noiseSprite.setTexture(noiseTexture);
				}
			}
			else if (event.type == Event::MouseWheelScrolled) {
				if (event.mouseWheelScroll.delta > 0) {
					defaultView.zoom(0.9f);
					window2.setView(defaultView);
				}
				else {
					defaultView.zoom(1.1f);
					window2.setView(defaultView);
				}
			}
			else if (event.type == Event::MouseButtonPressed) {
				if (event.mouseButton.button == Mouse::Left) {
					Vector2f mousePos = Vector2f(Mouse::getPosition(window2).x, Mouse::getPosition(window2).y);
					mousePos = window2.mapPixelToCoords(Vector2i(mousePos.x, mousePos.y), defaultView);
					//cout<<"Mouse position: "<<mousePos.x<<" "<<mousePos.y<<"\n";
					Vector2i tilePos = Vector2i(mousePos.x / tileSize, mousePos.y / tileSize);
					if(tilePos.x >= 0 && tilePos.x < width && tilePos.y >= 0 && tilePos.y < height)
						cout<<"Tile position: "<<tilePos.x<<" "<<tilePos.y<<" Tile type: "<<level[tilePos.y * width + tilePos.x]<<"\n";
				}
			}
		}

		p.update(currentTime.asSeconds());


		if (Mouse::isButtonPressed(Mouse::Right)) {
			Vector2f mousePos = Vector2f(Mouse::getPosition(window2).x, Mouse::getPosition(window2).y);
			mousePos = window2.mapPixelToCoords(Vector2i(mousePos.x, mousePos.y), defaultView);
			Vector2f lastMousePosCpy = lastMousePos;
			lastMousePosCpy = window2.mapPixelToCoords(Vector2i(lastMousePosCpy.x, lastMousePosCpy.y), defaultView);
			defaultView.move(lastMousePosCpy - mousePos);
			window2.setView(defaultView);
		}
		lastMousePos = Vector2f(Mouse::getPosition(window2).x, Mouse::getPosition(window2).y);



		window2.clear();
		//window2.draw(noiseSprite);
		window2.draw(map);
		p.drawPlayer(window2);
		window2.display();
	}

	return 0;
	*/

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

	// enet_address_set_host(&address, "127.0.0.1:8080");
	enet_address_set_host(&address, "172.205.150.168");

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

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
	float timeBetweenPackets = 1.f / tps;

	while (window.isOpen()) {
		currentTime = deltaClock.restart();
		Event event;

		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
			player.handleEvent(event);
			for (auto& p : playerMap) {
				p.second->handleEvent(event);
			}
			if (event.type == Event::MouseButtonPressed) {
				if (event.mouseButton.button == Mouse::Left) {
					Vector2i mousePos = Mouse::getPosition(window);
					Vector2f worldPos = window.mapPixelToCoords(mousePos);
					if (waterTileShape.getGlobalBounds().contains(worldPos) && player.getSelectedItemId() == ItemNames::CUP_EMPTY) {
						player.setSelectedItemId(ItemNames::CUP_WATER);
					}
				}
			}
		}

		msgLoop(client);

		player.update(currentTime.asSeconds());
		for (auto& p : playerMap) {
			p.second->update(currentTime.asSeconds());
		}

		packageTimeCounter += currentTime.asSeconds();
		if (packageTimeCounter >= timeBetweenPackets) {
			char message[13];
			packData(message, '1', CLIENTID, player.getPosition().x, player.getPosition().y);
			sendPacket(peer, message);
			packageTimeCounter = 0.0f;
		}

		window.clear();
		window.draw(map);
		window.draw(waterTileShape);
		player.drawPlayer(window);
		for (auto& p : playerMap) {
			p.second->drawPlayer(window);
		}
		window.display();
	}

	enet_peer_disconnect(peer, 0);
	cout << "Peer disconnected..." << "\n";

	while (enet_host_service(client, &event, 3000) > 0) {
		if (event.type == ENET_EVENT_TYPE_RECEIVE)
			enet_packet_destroy(event.packet);
		else if (event.type == ENET_EVENT_TYPE_DISCONNECT)
			cout << "Disconnection succeeded.\n";
	}

	return EXIT_SUCCESS;
}
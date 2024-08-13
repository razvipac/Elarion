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
#include "NetworkManager.h"

using namespace std;
using namespace sf;

map<int, Player*> playerMap;
Player* playerPointer;
int CLIENTID = -1;
int tps = 10;
Font font;

Vector2f mousePosInWorld;
Vector2f mousePosInUI;

void makeCameraFollowPlayer(View& view, Player& player, float deltaTime, float cameraSpeed) {
	Vector2f targetPosition = (player.getPosition() - view.getCenter()) * deltaTime * cameraSpeed + view.getCenter();
	view.setCenter(targetPosition);
}

int main() {
	cout << "Starting\n";
	srand(time(NULL));

	TextureManager::getInstance().loadTexture("PlayerIdle", "Resources/Sunnyside_World_Assets/Characters/Human/IDLE/base_idle_strip9.png");
	TextureManager::getInstance().loadTexture("PlayerWalk", "Resources/Sunnyside_World_Assets/Characters/Human/WALKING/base_walk_strip8.png");
	TextureManager::getInstance().loadTexture("PlayerRun", "Resources/Sunnyside_World_Assets/Characters/Human/RUN/base_run_strip8.png");
	TextureManager::getInstance().loadTexture("PlayerAttack", "Resources/Sunnyside_World_Assets/Characters/Human/ATTACK/base_attack_strip10.png");
	TextureManager::getInstance().loadTexture("PlayerHurt", "Resources/Sunnyside_World_Assets/Characters/Human/HURT/base_hurt_strip8.png");
	TextureManager::getInstance().loadTexture("PlayerDeath", "Resources/Sunnyside_World_Assets/Characters/Human/DEATH/base_death_strip13.png");
	
	TextureManager::getInstance().loadTexture("ItemIdle", "Resources/Sunnyside_World_Assets/Characters/Human/IDLE/tools_idle_strip9.png");
	TextureManager::getInstance().loadTexture("ItemWalk", "Resources/Sunnyside_World_Assets/Characters/Human/WALKING/tools_walk_strip8.png");
	TextureManager::getInstance().loadTexture("ItemRun", "Resources/Sunnyside_World_Assets/Characters/Human/RUN/tools_run_strip8.png");
	TextureManager::getInstance().loadTexture("ItemAttack", "Resources/Sunnyside_World_Assets/Characters/Human/ATTACK/tools_attack_strip10.png");
	TextureManager::getInstance().loadTexture("ItemHurt", "Resources/Sunnyside_World_Assets/Characters/Human/HURT/tools_hurt_strip8.png");
	TextureManager::getInstance().loadTexture("ItemDeath", "Resources/Sunnyside_World_Assets/Characters/Human/DEATH/tools_death_strip13.png");

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

	font.loadFromFile("Resources/Roboto-Black.ttf");

	NetworkManager& networkManager = NetworkManager::getInstance();

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

	View defaultView = window.getDefaultView();
	View uiView = window.getDefaultView();

	float cameraSpeed = 2.f;

	playerMap[-20] = new Player(-20);

	while (window.isOpen()) {
		mousePosInWorld = window.mapPixelToCoords(Mouse::getPosition(window));
		mousePosInUI = window.mapPixelToCoords(Mouse::getPosition(window), uiView);
		currentTime = deltaClock.restart();
		Event event;

		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
			//player.handleEvent(event, window);
			for (auto& p : playerMap) {
				p.second->handleEvent(event, window);
			}
			if (event.type == Event::MouseButtonPressed) {
				if (event.mouseButton.button == Mouse::Left) {
					if (waterTileShape.getGlobalBounds().contains(mousePosInWorld) && player.getSelectedItemId() == ItemNames::CUP_EMPTY) {
						player.setSelectedItemId(ItemNames::CUP_WATER);
					}
				}
			}
		}

		networkManager.msgLoop();

		//player.update(currentTime.asSeconds());
		for (auto& p : playerMap) {
			p.second->update(currentTime.asSeconds());
		}

		makeCameraFollowPlayer(defaultView, player, currentTime.asSeconds(), cameraSpeed);

		packageTimeCounter += currentTime.asSeconds();
		if (packageTimeCounter >= timeBetweenPackets) {
			char message[13];
			packMovementData(message, 1, CLIENTID, player.getPosition().x, player.getPosition().y);
			networkManager.sendPacket(message, 13, false);
			packageTimeCounter = 0.0f;
		}

		window.clear();
		window.draw(map);
		window.draw(waterTileShape);
		//player.draw(window);
		for (auto& p : playerMap)
			p.second->draw(window);
		window.setView(uiView);
		player.drawInventory(window);
		window.setView(defaultView);
		window.display();
	}

	return EXIT_SUCCESS;
}
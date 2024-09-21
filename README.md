# Elarion
Elarion is a simple 2D online multiplayer game built using enet for the networking and SFML for rendering. This repository contains all of the game's files, as well as the map editor used for making the world and the custom animator app used for building complex animation systems.

## Animator app
- Mimics Unity's animator app
- Ability to create complex state machines with transitions and conditions for dynamic animations
- Simple and intuitive UI
- Integrates the windows API for file browsing
- Saves and loads animator files using the .animator format and loads new animation states using the .animation format

## Map editor
- Ability to create complex maps using a lightweight tilemap system
- Includes 2 layers by default in order to allow the creation of better levels
- Simple and intuitive UI
- Integrates the windows API for file browsing
- Saves and loads map files using the .map format

## Game
Being a multiplayer game, it is split into 2 applications: a server and a client. The server syncs the movement and attacks over the network using a custom UDP protocol and uses interpolation and animation syncing on the client side in order to give the players a smooth and visually pleasing experience.

The game also features the ability to farm crops on certain tiles and a camera which smoothly follows the player around.

## Installation
The repository contains a total of 4 MSVS projects, for the game and its tools. Installation is simple, by just cloning the repository, but it does require Microsoft Visual Studio with the C++ add-ons to work.

## Team
The project was created by 2 people:
- [Alex Turculet](https://github.com/Androidus2)
- [Razvan Paraschiv](https://github.com/razvipac)

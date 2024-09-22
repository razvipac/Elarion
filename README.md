# Elarion
Elarion is an engaging 2D online multiplayer game developed using ENet for networking and SFML for rendering. The game combines multiplayer functionalities with farming mechanics and smooth animations, aiming for a pleasent experience while also enjoying the full potential of the gameplay. The repository includes powerful tools, namely a custom animator and map editor, which allow for straightforward modifications and/or extensions of the game's content. These services were built by trying to mimick the ones offered by Unity, the intention behind being to provide flexibility and ease of use.

## Features at first glance
- *Multiplayer*: Seamless online play with server-client architecture.
- *Custom Tools*: Integrates a map editor and an animator app to create, modify and fine-tune the game's assets.
- *Smooth Player Experience*: With synchronized animations and interpolated movement, the gameplay remains visually untroubled and reactive.
- *Farming Mechanics*: To enrichen the field of possibilities, players can plant and farm crops, thus adding depth and strategy to the gameplay.

## Animator app
The Elarion's Animator is a versatile tool inspired by Unity's own system, designed to simplify the management of the animations while offering impactful and complex animation workflows.
- *Complex State Machines*: Build animation state machines with transitions and conditions for dynamic control over your character's actions.
- *File System Integration*: Leverages the Windows API for file browsing and organisation.
- *Custom File Formats*: Save your work in .animator and .animation forms, enabling easy loading and modification of animation states.
- *User-Friendly UI*: Focus on creativity with an intuitive interface that simplifies navigation.
Whether you are creating simple animations or sophisticated sequences, the Animator is an indispensable tool.

## Map editor
The Elarion's Map Editor allows for easy creation of rich, layered 2D worlds using an efficient tilemap system.
- *Multi-Layer Mapping*: Support for multiple layers, with two available by default, giving creators the power over depth and detail.
- *Efficient Tilemap System*: Build and manage maps quickly with an optimized tile-based structure.
- *File System Integration*: Like the Animator app, it integrates Windows API for file handling.
- *Custom File Formats*: Maps are saved and loaded using the .map format, ensuring easy version control.
Create, edit and expand your game world by making use of this tool.

## Game Mechanics
Elarion features core mechanics that emphasize both cooperative and competitive multiplayer experiences:
- *Server-Client Architecture*: The game is split into two separate applications - a server and a client. The server handles synchronization of movements, attacks and events over the network using a custom UDP protocol.
- *Smooth Animation & Interpolation*: The client uses interpolation to sync movements, guaranteeing fluid player control.
- *Dynamic Camera*: The camera follows the player around, providing a polished visual experience.
Whether battling with friends or farming together, Elarion promises captivating and exciting adventures!

## Installation
Elarion's repository contains four MSVS projects (Server, Client, Animator App, Map Editor).
To install:
1. Clone the repository.
2. Open in Microsoft Visual Studio with the C++ add-ons installed.
3. Build and run the projects according to your setup.
4. Enjoy!

## Meet the Developers
The project was created by:
- [Alex Turculet](https://github.com/Androidus2)
- [Razvan Paraschiv](https://github.com/razvipac)

Together we have designed this game and its accompanying utility tools from the ground up.

#pragma once
#include "SFML/Graphics.hpp"
#include <map>
#include <iostream>

class TextureManager {
private:
	std::map<std::string, sf::Texture> textures;
	TextureManager();
public:
	void loadTexture(const std::string& name, const std::string& filename);
	sf::Texture& getRef(const std::string& texture);

	static TextureManager& getInstance() {
		static TextureManager instance;
		return instance;
	}
	TextureManager(TextureManager const&) = delete;
	void operator=(TextureManager const&) = delete;
};
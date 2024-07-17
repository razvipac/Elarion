#pragma once
#include "SFML/Graphics.hpp"

class TileMap : public sf::Drawable, public sf::Transformable {
private:
	sf::VertexArray vertices;
	sf::Texture tileset;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
public:
	bool load(const std::string& tilesetPath, sf::Vector2u tileSize, const int* tiles, int width, int height);
	void update(const int* tiles, sf::Vector2u tileSize, int width, int height);
};
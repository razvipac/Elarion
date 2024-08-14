#pragma once
#include "SFML/Graphics.hpp"

class TileMap : public sf::Drawable, public sf::Transformable {
private:
	const int width;
	const int height;
	int tileSize;
	int *level;
	sf::VertexArray vertices;
	sf::Texture tileset;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
public:
	TileMap();
	
	int getWidth() const;
	int getHeight() const;
	int getTileSize() const;
	const int* getLevel() const;
	int getTile(int x, int y) const;

	void setTileSize(int tileSize);
	void setLevel(int* level);
	void setTile(int x, int y, int tile);

	bool load(const std::string& tilesetPath);
	void update();
	
	~TileMap();
};
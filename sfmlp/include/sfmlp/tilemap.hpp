#pragma once

#include <string>
#include <SFML/System/Vector2.hpp>

namespace sf
{
	class Sprite;
	class RenderWindow;
}

class Path;

class TileMap
{
public:
	TileMap();
	~TileMap();
	static const int tileMapSize = 100;
	int mapWidth;
	int mapHeight;
	void Render(sf::RenderWindow& window);
	void SaveToFile(const std::string& fileName);
	void LoadFromFile(const std::string& fileName);
	bool WithinBounds(sf::Vector2i point);

	int* tiles;
	int* newTiles;
	sf::Sprite* tileTypes[10];
	void ResetToTile(int tileIndex);

	size_t index(int x, int y);

private:
	Path* path;

	std::string GetProperty(std::string line);
	int GetIntProperty(std::string line);
};
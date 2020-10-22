#include <tilemap.hpp>

#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>

TileMap::TileMap()
{
	mapWidth = 20;
	mapHeight = 20;

	tiles = new int[mapWidth * mapHeight];

	for (int x = 0; x < mapWidth; x++)
	{
		for (int y = 0; y < mapHeight; y++)
		{
			tiles[index(x, y)] = 0;

			if ((x + y) % 2 == 0)
			{
				tiles[index(x, y)] = 1;
			}
		}
	}
}

TileMap::~TileMap()
{
}

size_t TileMap::index(int x, int y)
{
	return x + mapWidth * y;
}

void TileMap::Render(sf::RenderWindow* window)
{
	for (int x = 0; x < mapWidth; x++)
	{
		for (int y = 0; y < mapHeight; y++)
		{
			if (tiles[index(x, y)] > 3)
				tiles[index(x, y)] = 3;

			sf::Sprite* sprite = tileTypes[tiles[index(x, y)]];
			sprite->setPosition(static_cast<float>(x * 32), static_cast<float>(y * 32));
			window->draw(*sprite);
		}
	}
}

std::string TileMap::GetProperty(std::string line)
{
	int rValue = 0;
	auto index = line.find_first_of(':');
	if (index != std::string::npos)
	{
		auto property = line.substr(0, index);
		return property;
	}
}

int TileMap::GetIntProperty(std::string line)
{
	int rValue = 0;
	auto index = line.find_first_of(':');
	if (index != std::string::npos)
	{
		auto value = line.substr(index + 1, line.size() - index);
		rValue = std::stoi(value);
	}

	return rValue;
}

void TileMap::LoadFromFile(const std::string& fileName)
{
	std::string line;
	std::ifstream myfile(fileName);

	if (myfile.is_open())
	{
		int x = 0;
		int y = 0;

		while (getline(myfile, line))
		{
			bool propertyFound = false;

			for (int i = 0; i < line.length(); i++)
			{
				if (GetProperty(line) == "width")
				{
					mapWidth = GetIntProperty(line);
					propertyFound = true;
					break;
				}

				if (GetProperty(line) == "height")
				{
					mapHeight = GetIntProperty(line);
					propertyFound = true;

					tiles = new int[mapWidth * mapHeight];

					break;
				}

				if (x >= mapWidth)
				{
					std::cout << "X is out of bounds, too many characters in line: " + std::to_string(i) << std::endl;
					break;
				}

				tiles[index(x, y)] = line[i] - '0';
				x++;
			}

			if (propertyFound)
				continue;

			x = 0;
			y++;

			if (y >= mapHeight)
			{
				std::cout << "Y is out of bounds, too many lines in file." << std::endl;
				break;
			}
		}
		myfile.close();
	}
}

bool TileMap::WithinBounds(sf::Vector2i point)
{
	return (point.x >= 0 && point.x < mapWidth
		&& point.y >= 0 && point.y < mapHeight);
}

void TileMap::SaveToFile(const std::string& fileName)
{
	std::string line;
	std::ofstream myfile(fileName);

	if (myfile.is_open())
	{
		myfile << "width:" << mapWidth << "\n";
		myfile << "height:" << mapHeight << "\n";

		for (int y = 0; y < mapHeight; y++)
		{
			for (int x = 0; x < mapWidth; x++)
			{
				line += std::to_string(tiles[index(x, y)]);
			}
			line += '\n';
		}

		myfile << line;

		myfile.close();
	}
}

void TileMap::ResetToTile(int tileIndex)
{
	if (tileIndex > 3)
	{
		tileIndex = 3;
	}

	for (int x = 0; x < tileMapSize; x++)
	{
		for (int y = 0; y < tileMapSize; y++)
		{
			tiles[index(x, y)] = tileIndex;
		}
	}
}
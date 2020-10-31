#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <structs.hpp>
#include <sfmlp/tilemap.hpp>

class Renderer
{
public:
	Renderer(EntityList entities, Map map)
	{
		this->entities = entities;

		for(int i = 0; i < entities.entities.size(); i++)
		{
			EntityData ent = entities.entities[i];
			LoadEntity(i, ent.texture);
		}

		for (int i = 0; i < map.tileTypes.size(); i++)
		{
			LoadTileType(i, map.tileTypes[i]);
		}

		std::copy(map.tileLayerBottom.begin(), map.tileLayerBottom.end(), tileMap.tiles);
	}

	void Render(GameState& state, sf::RenderWindow& window)
	{
		tileMap.Render(window);
		for (int i = 0; i < MAX_ENTITY_COUNT; i++)
		{
			Entity& entity = state.entities[i];
			DrawEntity(entity, window, 128);
		}
	}

	void DrawEntity(Entity& entity, sf::RenderWindow& window, int alpha = 255)
	{
		auto renderable = spriteTypes[entity.type];
		renderable.setPosition(entity.position.x, entity.position.y);
		auto white = sf::Color::White;
		white.a = alpha;
		renderable.setColor(white);
		window.draw(renderable);

		if (drawDirectionVectors)
		{
			sf::VertexArray lines(sf::Lines, 2);
			lines[0].position = sf::Vector2f(entity.position.x, entity.position.y);
			lines[1].position = sf::Vector2f(entity.position.x + (entity.direction.x * 20), entity.position.y + (entity.direction.y * 20));
			window.draw(lines);
		}
	}

	bool drawDirectionVectors = true;
private:

	void LoadEntity(int i, std::string fileName)
	{
		textures[i].loadFromFile(fileName);
		spriteTypes[i] = sf::Sprite(textures[i]);

		spriteTypes[i].setOrigin(
			textures[i].getSize().x * entities.entities[i].originPoint.x,
			textures[i].getSize().y * entities.entities[i].originPoint.y);
	}

	void LoadTileType(int i, std::string fileName)
	{
		tileTextures[i].loadFromFile(fileName);
		tileMap.tileTypes[i] = new sf::Sprite(tileTextures[i]);
	}

	sf::Sprite spriteTypes[100];
	sf::Texture textures[100];
	sf::Texture tileTextures[100];
	EntityList entities;
	Map map;
	TileMap tileMap;
};
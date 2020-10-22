#include <SFML\Graphics.hpp>
#include <enet\enet.h>
#include <stdint.h>
#include <game.hpp>
#include <networkclient.hpp>
#include <tilemap.hpp>

int main()
{
	sf::RenderWindow window(sf::VideoMode(640, 480), "Client");
	sf::Sprite spriteTypes[100];

	sf::Texture playerTexture;
	playerTexture.loadFromFile("Assets/enemy1.png");
	spriteTypes[0] = sf::Sprite(playerTexture);

	sf::Texture zTentacleTexture;
	zTentacleTexture.loadFromFile("Assets/ZTentacle.png");
	spriteTypes[99] = sf::Sprite(zTentacleTexture);

	sf::Texture projectile3;
	projectile3.loadFromFile("Assets/Projectile3.png");
	spriteTypes[98] = sf::Sprite(projectile3);

	TileMap map;
	map.tiles[map.index(5, 7)] = 2;
	map.tiles[map.index(5, 8)] = 2;
	map.tiles[map.index(4, 7)] = 2;
	map.tiles[map.index(4, 8)] = 2;
	sf::Texture testTexture;
	testTexture.loadFromFile("assets/grass.png");
	map.tileTypes[0] = new sf::Sprite(testTexture);

	sf::Texture testTexture2;
	testTexture2.loadFromFile("assets/dirt.png");
	map.tileTypes[1] = new sf::Sprite(testTexture2);

	sf::Texture testTexture3;
	testTexture3.loadFromFile("assets/water.png");
	map.tileTypes[2] = new sf::Sprite(testTexture3);

	sf::Texture testTexture4;
	testTexture4.loadFromFile("assets/cliff.png");
	map.tileTypes[3] = new sf::Sprite(testTexture4);

	NetworkClient client;
	client.Initialise();

	sf::View view;
	view.reset(sf::FloatRect(0, 0, 640, 480));

	sf::View minimapView;
	minimapView.reset(sf::FloatRect(0, 0, 640, 480));
	minimapView.setViewport(sf::FloatRect(0.75f, 0.f, 0.25f, 0.25f));


	bool hasFocus = true;
	while (window.isOpen())
	{
		
		sf::Event windowEvent;
		while (window.pollEvent(windowEvent)) 
		{
			if (windowEvent.type == sf::Event::Closed) 
			{
				window.close();
			}

			if (windowEvent.type == sf::Event::GainedFocus)
			{
				hasFocus = true;
			}

			if (windowEvent.type == sf::Event::LostFocus)
			{
				hasFocus = false;
			}
		}

		if (client.peer != nullptr)
		{
			client.Loop();
			view.setCenter(client.GetControlledEntity()->position.x, client.GetControlledEntity()->position.y);
		}

		window.clear(sf::Color::Black);

		if (hasFocus)
		{
			InputFrame playerFrame;
			playerFrame.type = InputType::MOVE;
			playerFrame.inputDirection = Vector2(0, 0);

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			{
				playerFrame.inputDirection.x -= 1;
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			{
				playerFrame.inputDirection.x += 1;
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			{
				playerFrame.inputDirection.y -= 1;
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			{
				playerFrame.inputDirection.y += 1;
			}

			if (playerFrame.inputDirection.x != 0 && playerFrame.inputDirection.y != 0)
			{
				playerFrame.inputDirection = playerFrame.inputDirection.Normalise();
			}

			client.SendInput(playerFrame);
		}

		window.setView(view);

		map.Render(&window);

		// Render game state.
		for (int i = 0; i < MAX_ENTITY_COUNT; i++)
		{
			auto renderable = spriteTypes[currentState->entities[i].type];
			renderable.setPosition(currentState->entities[i].position.x, currentState->entities[i].position.y);
			window.draw(renderable);
		}

		window.setView(minimapView);

		map.Render(&window);

		// Render game state.
		for (int i = 0; i < MAX_ENTITY_COUNT; i++)
		{
			auto renderable = spriteTypes[currentState->entities[i].type];
			renderable.setPosition(currentState->entities[i].position.x, currentState->entities[i].position.y);
			window.draw(renderable);
		}

		window.display();
	}

	client.Shutdown();
}
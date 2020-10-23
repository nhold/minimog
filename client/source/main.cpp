#include <SFML\Graphics.hpp>
#include <enet\enet.h>
#include <stdint.h>
#include <game.hpp>
#include <networkclient.hpp>
#include <tilemap.hpp>
#include <chrono>

sf::View getLetterboxView(sf::View view, int windowWidth, int windowHeight) {

	// Compares the aspect ratio of the window to the aspect ratio of the view,
	// and sets the view's viewport accordingly in order to archieve a letterbox effect.
	// A new view (with a new viewport set) is returned.

	float windowRatio = windowWidth / (float)windowHeight;
	float viewRatio = view.getSize().x / (float)view.getSize().y;
	float sizeX = 1;
	float sizeY = 1;
	float posX = 0;
	float posY = 0;

	bool horizontalSpacing = true;
	if (windowRatio < viewRatio)
		horizontalSpacing = false;

	// If horizontalSpacing is true, the black bars will appear on the left and right side.
	// Otherwise, the black bars will appear on the top and bottom.

	if (horizontalSpacing) {
		sizeX = viewRatio / windowRatio;
		posX = (1 - sizeX) / 2.f;
	}

	else {
		sizeY = windowRatio / viewRatio;
		posY = (1 - sizeY) / 2.f;
	}

	view.setViewport(sf::FloatRect(posX, posY, sizeX, sizeY));

	return view;
}

void LoadEntity(sf::Texture* textureTypes, sf::Sprite* spriteTypes, int i, const char* fileName)
{
	textureTypes[i].loadFromFile(fileName);
	spriteTypes[i] = sf::Sprite(textureTypes[i]);
	spriteTypes[i].setOrigin(textureTypes[i].getSize().x * entityDefinitions[i].originPoint.x,
		textureTypes[i].getSize().y * entityDefinitions[i].originPoint.y);
}

int main()
{
	sf::RenderWindow window(sf::VideoMode(640, 480), "Client");
	sf::Sprite spriteTypes[100];
	sf::Texture loadedTextures[100];

	LoadEntity(loadedTextures, spriteTypes, 0, "Assets/enemy1.png");
	LoadEntity(loadedTextures, spriteTypes, 99, "Assets/ZTentacle.png");
	LoadEntity(loadedTextures, spriteTypes, 98, "Assets/Projectile3.png");

	sf::Texture playerTexture;
	playerTexture.loadFromFile("Assets/enemy1.png");
	spriteTypes[0] = sf::Sprite(playerTexture);
	spriteTypes[0].setOrigin(playerTexture.getSize().x * entityDefinitions[0].originPoint.x, playerTexture.getSize().y * entityDefinitions[0].originPoint.y);

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
	view = getLetterboxView(view, 640, 480);

	sf::View minimapView;
	minimapView.reset(sf::FloatRect(0, 0, 640, 480));
	minimapView.setViewport(sf::FloatRect(0.75f, 0.f, 0.25f, 0.25f));


	bool hasFocus = true;

	using clock = std::chrono::high_resolution_clock;
	using ms = std::chrono::milliseconds;

	bool useKey = false;

	while (window.isOpen())
	{
		useKey = false;
		auto time_start = clock::now();
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

			if (windowEvent.type == sf::Event::Resized)
			{
				// update the view to the new size of the window
				view = getLetterboxView(view, windowEvent.size.width, windowEvent.size.height);
			}

			if (windowEvent.type == sf::Event::KeyReleased)
			{
				if (windowEvent.key.code == sf::Keyboard::Key::E)
				{
					useKey = true;
				}
			}
		}

		if (client.peer != nullptr)
		{
			client.Listen();
			view.setCenter(round(client.GetControlledEntity()->position.x), round(client.GetControlledEntity()->position.y));
		}

		window.clear(sf::Color::Black);

		if (hasFocus)
		{
			InputFrame playerFrame;
			playerFrame.type = InputType::NONE;
			playerFrame.inputDirection = Vector2(0, 0);

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			{
				playerFrame.inputDirection.x -= 1;
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			{
				playerFrame.inputDirection.x += 1;
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			{
				playerFrame.inputDirection.y -= 1;
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			{
				playerFrame.inputDirection.y += 1;
			}

			if (useKey)
			{
				playerFrame.type = InputType::PERFORMACTION;
				useKey = false;
			}

			if (playerFrame.inputDirection.x != 0 && playerFrame.inputDirection.y != 0)
			{
				playerFrame.inputDirection = playerFrame.inputDirection.Normalise();
			}

			client.SendInput(playerFrame);
			playerFrame.type = InputType::NONE;
		}

		window.setView(view);

		map.Render(&window);

		// Render game state.
		for (int i = 0; i < MAX_ENTITY_COUNT; i++)
		{
			auto renderable = spriteTypes[currentState->entities[i].type];
			renderable.setPosition(currentState->entities[i].position.x, currentState->entities[i].position.y);
			window.draw(renderable);
			sf::VertexArray lines(sf::Lines, 2);
			lines[0].position = sf::Vector2f(currentState->entities[i].position.x, currentState->entities[i].position.y);
			lines[1].position = sf::Vector2f(lines[0].position.x + (currentState->entities[i].direction.x * 20), lines[0].position.y + (currentState->entities[i].direction.y * 20));
			window.draw(lines);
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

		auto loop_time = clock::now() - time_start;
		while (loop_time < ms(16))
		{
			loop_time = clock::now() - time_start;
		}
	}

	client.Shutdown();
}
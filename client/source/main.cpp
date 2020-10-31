#include <SFML\Graphics.hpp>
#include <enet\enet.h>
#include <stdint.h>
#include <game.hpp>
#include <networkclient.hpp>
#include <chrono>
#include <json.hpp>
#include <Meta.h>
#include <JsonCast.h>
#include <iostream>
#include <iomanip>
#include <file.hpp>
#include <sfmlp\tilemap.hpp>
#include <sfmlp\viewhelper.hpp>
#include <renderer.hpp>
#include <input.hpp>
#include <iostream>

int main()
{
	EntityList list = GetFileToObject<EntityList>("assets/entities.json");
	Map mapDefine = GetFileToObject<Map>("assets/map.json");
	Renderer renderer(list, mapDefine);
	Input input;

	sf::Font font;
	font.loadFromFile("assets/PixelByzantine.ttf");
	sf::Text ping;
	ping.setFont(font);
	ping.setCharacterSize(20);
	ping.setColor(sf::Color(160, 32, 240));
	ping.setString("Ping: 0");
	ping.setPosition(0, 0);
	sf::RenderWindow window(sf::VideoMode(640, 480), "Client");

	NetworkClient client;
	client.Initialise();

	sf::View view;
	view.reset(sf::FloatRect(0, 0, 640, 480));
	view = GetLetterboxView(view, 640, 480);

	using clock = std::chrono::steady_clock;
	using tickRate = std::chrono::duration<clock::rep, std::ratio<1, 60>>;
	using networkUpdateRate = std::chrono::duration<clock::rep, std::ratio<10, 60>>;

	auto nextTick = clock::now() + tickRate{ 1 };

	int currentLerpIndex = 0;
	float perc = 0;

	Game::Global().entityDefinitions[0].speed = 2;

	sf::CircleShape shape(100);
	
	while (window.isOpen())
	{
		input.Reset();
		sf::Event windowEvent;
		while (window.pollEvent(windowEvent)) 
		{
			input.Process(windowEvent);

			if (windowEvent.type == sf::Event::Closed) 
			{
				window.close();
			}

			if (windowEvent.type == sf::Event::Resized)
			{
				view = GetLetterboxView(view, windowEvent.size.width, windowEvent.size.height);
			}			
		}

		if (client.peer != nullptr)
		{
			client.Listen();
			view.setCenter(round(client.GetLocalEntity()->position.x), round(client.GetLocalEntity()->position.y));

			if (input.hasFocus)
			{
				client.CaptureInput(input);
			}
		}

		// Process at same tickrate.
		auto now = clock::now();
		if (now >= nextTick)
		{
			auto diff = now - nextTick;
			client.ProcessInputs();
			nextTick += tickRate{ 1 } + diff;
		}

		window.clear(sf::Color::Black);

		window.setView(view);
		renderer.Render(*Game::Global().currentState, window);
		renderer.DrawEntity(client.player, window);
		window.setView(window.getDefaultView());
		if (client.peer != nullptr)
			ping.setString("latency " + std::to_string(client.peer->roundTripTime));
		window.draw(ping);
		window.display();

	}
	

	client.Shutdown();
}
#include <game.hpp>
#include <chrono>
#include <thread>
#include <networkserver.hpp>
#include <ztentaclecontroller.hpp>

int main()
{
	NetworkServer networkServer;
	networkServer.Initialise();

	using clock = std::chrono::steady_clock;
	using tickRate = std::chrono::duration<clock::rep, std::ratio<1, 60>>;
	using networkUpdateRate = std::chrono::duration<clock::rep, std::ratio<10, 60>>;

	auto nextTick = clock::now() + tickRate{ 1 };
	auto nextNetworkUpdate = clock::now() + networkUpdateRate{ 1 };

	// HACK: Enforce player speed
	Game::Global().entityDefinitions[0].speed = 2;

	while (networkServer.isRunning)
	{
		networkServer.Listen();

		auto now = clock::now();
		if (now >= nextTick)
		{
			auto diff = now - nextTick;
			networkServer.ProcessInput();
			nextTick += tickRate{ 1 } + diff;
		}

		now = clock::now();
		if (now >= nextNetworkUpdate)
		{
			auto diff = now - nextTick;
			networkServer.SendGameState();
			nextNetworkUpdate += networkUpdateRate{ 1 } + diff;
		}
	}

	networkServer.Shutdown();
}
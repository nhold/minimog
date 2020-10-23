#include <game.hpp>
#include <chrono>
#include <thread>
#include <networkserver.hpp>

int main()
{
	NetworkServer networkServer;
	networkServer.Initialise();

	using clock = std::chrono::steady_clock;
	using tickRate = std::chrono::duration<clock::rep, std::ratio<1, 60>>;
	using networkUpdateRate = std::chrono::duration<clock::rep, std::ratio<1, 60>>;

	auto nextTick = clock::now() + tickRate{ 1 };
	auto nextNetworkUpdate = clock::now() + networkUpdateRate{ 1 };

	EpicZStage1 stage;
	stage.Initialise();

	entityDefinitions[0].speed = 2;

	while (networkServer.isRunning)
	{
		networkServer.ReceiveInputs();

		auto now = clock::now();

		if (now >= nextTick)
		{
			auto diff = nextTick - now;
			stage.Process();
			Simulate(0, MAX_ENTITY_COUNT);
			nextTick += tickRate{ 1 } + diff;
		}

		now = clock::now();
		if (now >= nextNetworkUpdate)
		{
			auto diff = nextTick - now;
			networkServer.SendGameState();
			nextNetworkUpdate += networkUpdateRate{ 1 } + diff;
		}
	}

	networkServer.Shutdown();
}
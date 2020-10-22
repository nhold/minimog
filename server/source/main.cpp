#include <game.hpp>
#include <chrono>
#include <thread>
#include <networkserver.hpp>

int main()
{
	NetworkServer networkServer;
	networkServer.Initialise();

	using clock = std::chrono::high_resolution_clock;
	using ms = std::chrono::milliseconds;

	while (networkServer.isRunning)
	{
		auto time_start = clock::now();

		networkServer.ReceiveInputs();
		Simulate(0, MAX_ENTITY_COUNT);
		networkServer.SendGameState();

		auto loop_time = clock::now() - time_start;
		while (loop_time < ms(16))
		{
			loop_time = clock::now() - time_start;
		}
	}

	networkServer.Shutdown();
}
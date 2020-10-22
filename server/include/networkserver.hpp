#pragma once

#include <stdint.h>
#include <enet/enet.h>
#include <game.hpp>
#include <chrono>

constexpr uint16_t MAX_PLAYER_COUNT = 100;

class NetworkServer
{
public:	
	bool isRunning = true;
	uint16_t currentPlayerCount = 0;
	uint16_t playerIndices[MAX_PLAYER_COUNT];
	ENetHost* server;

	void Initialise()
	{
		enet_initialize();

		ENetAddress address;
		

		address.host = ENET_HOST_ANY;
		address.port = 9999;

		server = enet_host_create(&address, 32, 4, 0, 0);
		enet_host_compress_with_range_coder(server);
		if (server == NULL)
		{
			fprintf(stderr, "An error occurred while trying to create an ENet server host.\n");
			exit(EXIT_FAILURE);
		}

		// First few entities are players.
		for (int i = 0; i < MAX_PLAYER_COUNT; i++)
		{
			playerIndices[i] = i;
		}
	}

	void ReceiveInputs()
	{
		ENetEvent event;
		while (enet_host_service(server, &event, 0) > 0)
		{
			ENetPacket* pack = nullptr;

			switch (event.type)
			{
			case ENET_EVENT_TYPE_CONNECT:
				printf("Giving id: %i\n", playerIndices[currentPlayerCount]);
				currentState->entities[playerIndices[currentPlayerCount]].type = 0;
				currentState->entities[playerIndices[currentPlayerCount]].speed = 5.f;
				pack = enet_packet_create(&playerIndices[currentPlayerCount], sizeof(uint16_t), ENET_PACKET_FLAG_RELIABLE);
				enet_peer_send(event.peer, 0, pack);
				currentPlayerCount++;

				break;
			case ENET_EVENT_TYPE_RECEIVE:
				if (event.channelID == 1)
				{
					Message* mess = (Message*)event.packet->data;
					ProcessMessage(*mess);
				}

				enet_packet_destroy(event.packet);
				break;

			case ENET_EVENT_TYPE_DISCONNECT:
				printf("%s disconnected.\n", event.peer->data);
				currentPlayerCount--;
			}
		}
	}

	void SendGameState()
	{
		ENetPacket* pack = enet_packet_create(currentState, sizeof(GameState), ENET_PACKET_FLAG_RELIABLE);
		enet_host_broadcast(server, 2, pack);
		currentState->frame++;
	}

	void Shutdown()
	{
		enet_host_destroy(server);
		enet_deinitialize();
	}
};
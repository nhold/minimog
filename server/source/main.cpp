#include <tilemapdata.hpp>
#include <iostream>
#include <json.hpp>
#include <game.hpp>
#include <enet\enet.h>
#include <stdint.h>

constexpr uint16_t MAX_PLAYER_COUNT = 100;

uint16_t playerIndices[MAX_PLAYER_COUNT];
uint16_t currentCount = 0;
bool isRunning = true;

int main()
{
	enet_initialize();
	ENetAddress address;
	ENetHost * server;
	/* Bind the server to the default localhost.     */
	/* A specific host address can be specified by   */
	/* enet_address_set_host (& address, "x.x.x.x"); */
	address.host = ENET_HOST_ANY;
	/* Bind the server to port 1234. */
	address.port = 9999;
	server = enet_host_create(&address /* the address to bind the server host to */,
		32      /* allow up to 32 clients and/or outgoing connections */,
		3      /* allow up to 2 channels to be used, 0 and 1 */,
		0      /* assume any amount of incoming bandwidth */,
		0      /* assume any amount of outgoing bandwidth */);

	if (server == NULL)
	{
		fprintf(stderr,
			"An error occurred while trying to create an ENet server host.\n");
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < MAX_PLAYER_COUNT; i++)
	{
		playerIndices[i] = 15;
	}

	ENetEvent event;
	while (isRunning)
	{
		while (enet_host_service(server, &event, 1000) > 0)
		{
			ENetPacket* pack = nullptr;
			printf("Something recieved");
			switch (event.type)
			{
			case ENET_EVENT_TYPE_CONNECT:
				printf("A new client connected from %x:%u.\n",
					event.peer->address.host,
					event.peer->address.port);

				// Tell them what ID they are 
				pack = enet_packet_create(&playerIndices[currentCount], sizeof(uint16_t), ENET_PACKET_FLAG_RELIABLE);
				enet_peer_send(event.peer, 0, pack);
				currentCount++;
				break;
			case ENET_EVENT_TYPE_RECEIVE:
				printf("A packet of length %u containing %s was received from %s on channel %u.\n",
					event.packet->dataLength,
					event.packet->data,
					event.peer->data,
					event.channelID);

				// The server can only get input messages.
				if (event.channelID == 1)
				{
					Message* mess = (Message*)event.packet->data;
					ProcessMessage(*mess);
				}

				/* Clean up the packet now that we're done using it. */
				enet_packet_destroy(event.packet);

				break;

			case ENET_EVENT_TYPE_DISCONNECT:
				printf("%s disconnected.\n", event.peer->data);
				currentCount--;
			}
		}

		Tick(0, 900);
		ENetPacket* pack = enet_packet_create(entities, MAX_ENTITY_COUNT * sizeof(Entity), ENET_PACKET_FLAG_RELIABLE);
		enet_host_broadcast(server, 2, pack);
	}

	enet_host_destroy(server);
	enet_deinitialize();
}
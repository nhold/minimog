#pragma once

#include <enet/enet.h>
#include <game.hpp>
#include <stdio.h>

class NetworkClient
{
public:
	ENetHost* client{};
	ENetEvent event{};
	ENetPeer* peer;
	int entityId = 0;
	ENetPacket* lastPacket = nullptr;

	NetworkClient()
	{
	}

	void Initialise()
	{
		enet_initialize();

		client = enet_host_create(NULL, 1, 3, 0, 0);
		enet_host_compress_with_range_coder(client);
		if (client == NULL)
		{
			fprintf(stderr, "An error occurred while trying to create an ENet client host.\n");
			exit(EXIT_FAILURE);
		}

		ENetAddress address;
	

		enet_address_set_host(&address, "127.0.0.1");
		address.port = 9999;

		/* Initiate the connection, allocating the two channels 0 and 1. */
		peer = enet_host_connect(client, &address, 3, 0);

		if (peer == NULL)
		{
			fprintf(stderr,
				"No available peers for initiating an ENet connection.\n");
			exit(EXIT_FAILURE);
		}

		/* Wait up to 5 seconds for the connection attempt to succeed. */
		if (enet_host_service(client, &event, 5000) > 0 &&
			event.type == ENET_EVENT_TYPE_CONNECT)
		{
			puts("Successfully connected to server.");
		}
		else
		{
			enet_peer_reset(peer);
			puts("Failed to connect to server.");
		}
	}

	void Listen()
	{
		while (enet_host_service(client, &event, 0) > 0)
		{
			switch (event.type)
			{
			case ENET_EVENT_TYPE_CONNECT:
				break;
			case ENET_EVENT_TYPE_RECEIVE:
		
				// This is the index we control.
				if (event.channelID == 0)
				{
					entityId = *event.packet->data;
					enet_packet_destroy(event.packet);
				}

				if (event.channelID == 2)
				{
					if (lastPacket != nullptr)
						enet_packet_destroy(lastPacket);

					currentState = (GameState*)event.packet->data;
					lastPacket = event.packet;
				}

				break;

			case ENET_EVENT_TYPE_DISCONNECT:
				break;
			}
		}
	}

	void SendInput(InputFrame frameTosend)
	{
		if (frameTosend.inputDirection.x == 0 && frameTosend.inputDirection.y == 0 && frameTosend.type == InputType::NONE)
		{
			return;
		}

		inputFrame[0] = frameTosend;
		Message message;
		message.frame = frameTosend;
		message.entityId = entityId;
		auto pack = enet_packet_create(&message, sizeof(Message), ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(peer, 1, pack);
	}

	void Shutdown()
	{
		enet_host_destroy(client);
		enet_deinitialize();
	}

	Entity* GetControlledEntity()
	{
		return &currentState->entities[entityId];
	}

};
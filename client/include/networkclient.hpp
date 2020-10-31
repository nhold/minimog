#pragma once

#include <enet/enet.h>
#include <game.hpp>
#include <stdio.h>
#include <input.hpp>
#include <algorithm>

struct OldData
{
	Entity processedState;
	InputFrame input;
};

class NetworkClient
{
public:
	ENetHost* client{};
	ENetEvent event{};
	ENetPeer* peer;
	JoinedMessage joinedMessage{};
	ENetPacket* lastPacket = nullptr;
	uint32_t lastProcessedInput = 0;
	int sequence = 0;
	std::vector<UpdateGameMessage> serverUpdates{};
	std::vector<InputFrame> inputs;
	std::vector<OldData> oldData;

	Entity player;

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
					joinedMessage = *(JoinedMessage*)event.packet->data;
					Game::Global().currentState->entities[joinedMessage.controlledId] = joinedMessage.snappedEnt;
					enet_packet_destroy(event.packet);
				}

				if (event.channelID == 2)
				{
					UpdateGameMessage* message = (UpdateGameMessage*)event.packet->data;
					serverUpdates.push_back(*message);
					memcpy(Game::Global().currentState, &message->state, sizeof(GameState));
					enet_packet_destroy(event.packet);
					Reconcile();
				}

				break;

			case ENET_EVENT_TYPE_DISCONNECT:
				break;
			}
		}
	}

	void Reconcile()
	{
		auto latestUpdate = serverUpdates.back();
		auto latestUpdateEntity = latestUpdate.state.entities[joinedMessage.controlledId];

		auto found = std::find_if(oldData.begin(), oldData.end(), [&](const OldData& s)
			{
				return s.input.sequence == latestUpdate.lastProcessedInput;
			});

		if (found != oldData.end())
		{
			auto b = *found;
			auto delta = b.processedState.position - latestUpdateEntity.position;
			if (delta.SqrMagnitude() == 0)
			{
				printf("Approved Input Sequence: %i\n", b.input.sequence);
				auto seq = b.input.sequence;

				// Clear old data, we are approved.
				std::remove_if(oldData.begin(), oldData.end(), [&](OldData d) 
					{
						return d.input.sequence < seq;
					});

				std::remove_if(inputs.begin(), inputs.end(), [&](InputFrame d)
					{
						return d.sequence < seq;
					});

				serverUpdates.pop_back();
			}
			else
			{
				printf("Rejected Input Sequence: %i\n EXP X,Y: %f,%f TRUE X,Y: %f,%f delta: %f\n\n", 
					b.input.sequence, 
					b.processedState.position.x, 
					b.processedState.position.y, 
					latestUpdate.state.entities[joinedMessage.controlledId].position.x, 
					latestUpdate.state.entities[joinedMessage.controlledId].position.y,
					delta.SqrMagnitude());
			}
		}
		else
		{
		}

	}

	void CaptureInput(Input input)
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

		if (input.justReleased[sf::Keyboard::E])
		{
			playerFrame.type = InputType::PERFORMACTION;
		}

		if (playerFrame.inputDirection.x != 0 && playerFrame.inputDirection.y != 0)
		{
			playerFrame.inputDirection = playerFrame.inputDirection.Normalise();
		}

		playerFrame.sequence = sequence;
		sequence++;

		if (!playerFrame.IsEmpty())
		{
			inputs.push_back(playerFrame);
			SendInput(playerFrame);
		}
	}

	void ProcessInputs()
	{
		for each (auto var in inputs)
		{
			if (var.sequence > lastProcessedInput)
			{
				OldData data;
				data.input = var;
				data.processedState = player;
				
				Game::Global().Movement(&var, &player, &Game::Global().entityDefinitions[0]);
				lastProcessedInput = var.sequence;
				oldData.push_back(data);
				printf("Processed Input Seq: %i. X:%f Y:%f\n", lastProcessedInput, player.position.x, player.position.y);
			}
		}
	}

	void SendInput(InputFrame frameTosend)
	{
		InputMessage message;
		message.frame = frameTosend;
		message.entityId = joinedMessage.controlledId;
		auto pack = enet_packet_create(&message, sizeof(InputMessage), ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(peer, 1, pack);
	}

	void Shutdown()
	{
		enet_host_destroy(client);
		enet_deinitialize();
	}

	Entity* GetLatestServerEntity()
	{
		return &Game::Global().currentState->entities[joinedMessage.controlledId];
	}

	Entity* GetLocalEntity()
	{
		return &player;
	}

};
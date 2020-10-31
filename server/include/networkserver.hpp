#pragma once

#include <stdint.h>
#include <enet/enet.h>
#include <game.hpp>
#include <chrono>
#include <vector>

constexpr uint16_t MAX_PLAYER_COUNT = 100;

struct PlayerData
{
	int32_t lastprocessedInput;
	float lastProcessedTime;
	uint16_t entityId;
	ENetPeer* connection;
	std::vector<InputFrame> inputFrames{};
};

class NetworkServer
{
public:	
	bool isRunning = true;
	uint16_t currentPlayerCount = 0;
	uint16_t playerIndices[MAX_PLAYER_COUNT];
	ENetHost* server;
	float currentTime;

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

	void Listen()
	{
		ENetEvent event;
		while (enet_host_service(server, &event, 0) > 0)
		{
			ENetPacket* pack = nullptr;

			switch (event.type)
			{
			case ENET_EVENT_TYPE_CONNECT:
				OnPlayerConnected(event.peer);
				enet_packet_destroy(event.packet);
				break;
			case ENET_EVENT_TYPE_RECEIVE:
				if (event.channelID == PLAYER_INPUT_CHANNEL)
				{
					AddPlayerInput(event);
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
		UpdateGameMessage message;
		message.state = *Game::Global().currentState;
		
		for (auto& [netId, playerData] : playerConnections)
		{
			message.lastProcessedInput = playerData.lastprocessedInput;
			printf("Sending last processed input: %i\n", message.lastProcessedInput);
			SendPacketTo(netId, &message, UPDATE_STATE_CHANNEL, ENET_PACKET_FLAG_RELIABLE);
		}
	}

	void ProcessInput()
	{
		for (auto& [netId, playerData] : playerConnections)
		{
			auto startingInputSeq = playerData.lastprocessedInput;
			
			for each (auto inputs in playerData.inputFrames)
			{
				auto entity = &Game::Global().currentState->entities[playerData.entityId];
				Game::Global().Movement(&inputs, entity, &Game::Global().entityDefinitions[0]);
				playerData.lastprocessedInput = inputs.sequence;
				playerData.lastProcessedTime = inputs.time;
			}

			if (startingInputSeq != playerData.lastprocessedInput)
			{
				printf("Processed Sequence %i -> %i\n", startingInputSeq, playerData.lastprocessedInput);
			}

			playerData.inputFrames.clear();
		}
	}

	void Shutdown()
	{
		enet_host_destroy(server);
		enet_deinitialize();
	}

private:
	void OnPlayerConnected(ENetPeer* enetPeer)
	{
		puts("Player Connected...");

		AddPlayer(enetPeer);

		JoinedMessage message;
		message.controlledId = playerConnections[enetPeer->incomingPeerID].entityId;
		message.serverTime = currentTime;
		message.snappedEnt = Game::Global().currentState->entities[playerConnections[enetPeer->incomingPeerID].entityId];

		SendPacketTo(enetPeer->incomingPeerID, &message, PLAYER_JOINED_CHANNEL, ENET_PACKET_FLAG_RELIABLE);
	}

	void AddPlayer(ENetPeer* enetPeer)
	{
		// Add player data to consistent map.
		PlayerData data;
		data.connection = enetPeer;
		data.lastprocessedInput = 0;
		data.entityId = playerIndices[currentPlayerCount];

		playerConnections[enetPeer->incomingPeerID] = data;

		// Set current state entity to latest. 0 - Player.
		Game::Global().currentState->entities[data.entityId].type = 0;

		currentPlayerCount++;

		printf("Added Player(%i). Network ID:%i and Entity ID:%i", currentPlayerCount, enetPeer->incomingPeerID, playerConnections);
	}

	void AddPlayerInput(ENetEvent& enetEvent)
	{
		InputMessage mess = *(InputMessage*)enetEvent.packet->data;
		playerConnections[enetEvent.peer->incomingPeerID].inputFrames.push_back(mess.frame);
	}

	template <class T>
	void SendPacketTo(enet_uint16 networkId, T* message, enet_uint8 channel,  _ENetPacketFlag flag)
	{
		ENetPacket* packet = enet_packet_create(message, sizeof(T), flag);
		enet_peer_send(playerConnections[networkId].connection, channel, packet);
	}

	std::map<enet_uint16, PlayerData> playerConnections;
};
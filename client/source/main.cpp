#include <SFML\Graphics.hpp>
#include <enet\enet.h>
#include <stdint.h>

uint16_t index;

int main()
{
	sf::RenderWindow window(sf::VideoMode(640, 480), "");
	enet_initialize();

	ENetHost * client;
	client = enet_host_create(NULL, 1, 3, 0, 0);

	if (client == NULL)
	{
		fprintf(stderr, "An error occurred while trying to create an ENet client host.\n");
		exit(EXIT_FAILURE);
	}

	ENetAddress address;
	ENetEvent event;
	ENetPeer *peer;

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
		puts("Connection to some.server.net:1234 succeeded.");
	}
	else
	{
		/* Either the 5 seconds are up or a disconnect event was */
		/* received. Reset the peer in the event the 5 seconds   */
		/* had run out without any significant event.            */
		enet_peer_reset(peer);
		puts("Connection to some.server.net:1234 failed.");
	}

	while (window.isOpen()) {
		sf::Event windowEvent;
		while (window.pollEvent(windowEvent)) {

			if (windowEvent.type == sf::Event::Closed) {
				window.close();
			}
		}

		while (enet_host_service(client, &event, 1000) > 0)
		{
			ENetPacket* pack = nullptr;
			
			switch (event.type)
			{
			case ENET_EVENT_TYPE_CONNECT:
				break;
			case ENET_EVENT_TYPE_RECEIVE:
				printf("A packet of length %u containing %s was received from %s on channel %u.\n",
					event.packet->dataLength,
					event.packet->data,
					event.peer->data,
					event.channelID);

				if (event.channelID == 0)
				{
					index = *event.packet->data;
				}

				/* Clean up the packet now that we're done using it. */
				enet_packet_destroy(event.packet);

				break;

			case ENET_EVENT_TYPE_DISCONNECT:
				break;
			}
		}

		window.clear();
		window.display();
	}

	enet_host_destroy(client);
	enet_deinitialize();
}
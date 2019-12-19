/*
 * enet_client.cpp
 *
 *  Created on: 2019Äê12ÔÂ19ÈÕ
 *      Author: poplarli
 */

#include <iostream>
#include <enet/enet.h>
#include <cstring>


int main(int argc, char** argv){

	if(argc != 3){
		std::cerr << "Usage <ip> <port>" << std::endl;
		return 1;
	}

	if (enet_initialize () != 0)
	{
		std::cerr << "An error occurred while initializing ENet.\n";
		return 1;
	}

	atexit (enet_deinitialize);

	ENetHost * client = enet_host_create(nullptr, 1, 2, 0, 0);
	if(client == nullptr){
		std::cerr << "An error occurred while trying to create an ENet server host.\n";
		return 1;
	}


	ENetAddress address;
	enet_address_set_host(&address, argv[1]);
	address.port = atoi(argv[2]);

	ENetPeer * peer = enet_host_connect(client, &address, 2, 0);
	if(peer == nullptr){
		std::cerr << "An error occurred while connecting\n";
		return 1;
	}

	int cnt = 0;
	ENetEvent event;
	while(cnt < 5){
		while(enet_host_service(client, &event, 0) && cnt < 5){
				switch(event.type){
				case ENET_EVENT_TYPE_CONNECT:
				{
					std::cout << "connect suc " << event.peer->address.host << ":" << event.peer->address.port << std::endl;
					//event.peer->data = "server information";

					auto content = "i am client";
					ENetPacket* packet = enet_packet_create(content, std::strlen(content), ENET_PACKET_FLAG_RELIABLE);
					enet_peer_send(event.peer, 0, packet);
					enet_host_flush(client);

					break;
				}

				case ENET_EVENT_TYPE_RECEIVE:
				{
				  printf ("A packet of length %u containing %s was received from %s on channel %u.\n",
								event.packet -> dataLength,
								event.packet -> data,
								event.peer -> data,
								event.channelID);

					enet_packet_destroy(event.packet);

					auto content = "hello server";
					ENetPacket* packet = enet_packet_create(content, std::strlen(content), ENET_PACKET_FLAG_RELIABLE);
					enet_peer_send(event.peer, 0, packet);
					enet_host_flush(client);

					++cnt;
					break;
				}
				case ENET_EVENT_TYPE_DISCONNECT:
				{
					std::cout << event.peer->data << " disconnected" << std::endl;
					event.peer->data = nullptr;
					break;
				}
				default:
					break;
			}
		}
	}


	return 0;
}



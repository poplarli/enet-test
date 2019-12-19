/*
 * enet_server.cpp
 *
 *  Created on: 2019Äê12ÔÂ19ÈÕ
 *      Author: poplarli
 */


#include <iostream>
#include <cstring>
#include "enet_server.h"

server::server(unsigned port) : server_(nullptr){

	address_.host = ENET_HOST_ANY;
	address_.port = port;
}

void server::run(){

	if (enet_initialize () != 0)
	{
		std::cerr << "An error occurred while initializing ENet.\n";
		return;
	}

	atexit (enet_deinitialize);


	server_ = enet_host_create(&address_, 2, 2, 0, 0);
	if(server_ == nullptr){
		std::cerr << "An error occurred while trying to create an ENet server host.\n";
		return;
	}

	ENetEvent event;
	while(1){
		while(enet_host_service(server_, &event, 1000) > 0){
				switch(event.type){
				case ENET_EVENT_TYPE_CONNECT:
				{
					std::cout << "a new client connected from " << event.peer->address.host << ":" << event.peer->address.port << std::endl;
					std::string cli_info = "Client information";
					event.peer->data = const_cast<char*>(cli_info.c_str());
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

					auto content = "hello client";
					ENetPacket* packet = enet_packet_create(content, std::strlen(content) + 1, ENET_PACKET_FLAG_RELIABLE);
					enet_peer_send(event.peer, 0, packet);

					enet_host_flush (server_);

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

}

server::~server(){
	enet_host_destroy(server_);
}


int main(int argc, char** argv){

	if(argc != 2){
		std::cerr << "Usage server <Port> " << std::endl;
		return 1;
	}

	server s(atoi(argv[1]));
	s.run();

	return 0;
}

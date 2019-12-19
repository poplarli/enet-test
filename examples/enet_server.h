/*
 * enet_server.h
 *
 *  Created on: 2019Äê12ÔÂ19ÈÕ
 *      Author: poplarli
 */

#ifndef ENET_SERVER_H_
#define ENET_SERVER_H_

#include <enet/enet.h>
#include <string>
#include <memory>
#include <functional>

class server{
public:
	server(const server&) = delete;
	server& operator=(const server&) = delete;
	server(unsigned port);

	server(server&&) = default;
	server& operator=(server&&) = default;

	~server();

	void run();

private:

	ENetAddress address_;
	ENetHost * server_;
};



#endif /* ENET_SERVER_H_ */

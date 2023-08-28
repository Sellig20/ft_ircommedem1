#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <map>
#include <iostream>
#include <cstring>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <vector>
#include "Server.hpp"
#include "Client.hpp"
// class Server;

class Client;

class Channel : public Server
{
	public :
		Channel();
		~Channel();
		Channel(Client *my_client);
		void setNameChannel(std::string nameToBe);
		const std::string getNameChannel();

	private :
		std::map<Client*, std::string> _clientListForChan;
		// std::string	my_channel;
		std::string nameChannel;
};

#endif

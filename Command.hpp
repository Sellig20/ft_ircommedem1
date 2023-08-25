#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <string>
#include <iostream>
#include <vector>
#include <sys/epoll.h>
#include <strings.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstdio>
#include "Server.hpp"
#include "Client.hpp"

class Command : public Server
{
	public :
		Command();
		~Command();
		Command(std::string com, std::string leftover, Client *my_client);
		void capls();
		void user();
		void nick();
		void pass();
		void ping();
		void mode();

	private :
		typedef void (Command::*fct)(void);
};

#endif

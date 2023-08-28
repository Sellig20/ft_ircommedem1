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
		void capls(std::string leftovers);
		void user(std::string leftovers);
		void nick(std::string leftovers);
		void pass(std::string leftovers);
		void ping(std::string leftovers);
		void mode(std::string leftovers);

		//zanot
		void invite(std::string leftovers);
		void join(std::string leftovers);
		void kick(std::string leftovers);
		void list(std::string leftovers);
		void names(std::string leftovers);
		void oper(std::string leftovers);
		void part(std::string leftovers);
		void privmsg(std::string leftovers);
		void topic(std::string leftovers);

	private :
		typedef void (Command::*fct)(std::string leftovers);
};

#endif

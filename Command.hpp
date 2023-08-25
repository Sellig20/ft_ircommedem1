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
#include <sstream>
#include "Server.hpp"
#include "Client.hpp"

class Command : public Server
{
	public :
		Command();
		~Command();
		Command(std::string &com, Client *_my_client);
		void capls();
		void user();
		void nick();
		void pass();
		void ping();
		void mode();

		void setIs_ready(bool _is_ready);

		const std::string getResponseBuffer() const;
		bool getIs_ready() const;
		bool getIs_Not_Accepted() const;
		const std::string getErrorcode() const;
		bool containsOnlySpaces(const std::string& str);
		bool is_token_valid(std::string nick);
		int parseUserCommand(const std::string &input, Client *my_client);


		std::string extractAfterUppercase(const std::string& input);

	private :
		typedef void (Command::*fct)(void);
		std::string response_buffer;
		std::string command_name;
		std::string command_content;
		std::string error_code;
		Client *my_client;
		bool				is_not_accepted;
		bool				is_ready;
};

#endif

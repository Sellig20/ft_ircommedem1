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
		Channel(std::string num, std::string name);
		void setNameChannel(std::string nameToBe);
		void setTopicChannel(std::string topicToBe);
		void addMember(const std::string& member);
		bool isMember(std::string memberName);
		const std::string getNameChannel();
		const std::string getNumChannel();
		const std::string getTopicChannel();
		const std::vector<std::string> getMemberOfThisChan();

	private :
		std::map<Client*, std::string> _clientListForChan;
		std::string nameChannel;
		std::string numChannel;
		std::string topicChannel;
		std::vector<std::string> memberOfThisChan;
};

std::ostream& operator<<(std::ostream& o, const std::vector<std::string>& memberVector);

#endif

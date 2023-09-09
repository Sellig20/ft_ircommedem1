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
#include "Channel.hpp"

#define SINGLE_SEND 11
#define NOT_ALL_SEND 12
#define ALL_SEND 13
#define NO_SEND 14


class Command : public Server
{
	public :
		Command();
		~Command();
		Command(std::string &com, Client *_my_client);

		int								getStatus(void);
		void							setStatus(int _status);
		std::map<std::string, std::string>	getChanKey();
		std::vector<std::string>			getNumActVecChan();


		std::string						builtReasonWhy(std::vector<std::string> reason);
		void							invite();
		void							join();
		void							kick();
		void							list();
		void							names();
		void							part();
		void							privmsg();
		void							topic();
		void							mode();
		void							capls();
		void							user();
		void							nick();
		void							pass();
		void							ping();
		void							whois();
		void							quit();
		void							who();

		bool 							containsOnlySpaces(const std::string& str);
		bool 							is_token_valid(std::string nick);
		int 							parseUserCommand(const std::string &input, Client *my_client);

		void 							fill_error_need_more_params(Command *my_command);
		void 							fill_error_password_mismatch(Command *my_command);
		void 							fill_error_already_registered(void);
		void							fill_no_such_nick(void);

		std::string						give_back_all_users_names(std::vector<std::string> members_of_chans, std::string channel_name);
		std::string						give_all_names(std::vector<std::string> members_of_chans, std::string channel_name);
		std::vector<std::string>		kick_user(std::vector<std::string> memberOfThisChan, std::string userNickname);
		
		void							broadcast_to_all_channels_user_is_in(Client *my_client);
		void							send_to_all_users_from_chan(Client *my_client, std::string nameChan, std::vector<std::string> members);
		Channel *						create_new_channel_from_scratch(std::string nameChan, std::string client_nick, Client *my_client);
		std::vector<std::string>		split_leftovers_by_comas(std::string &command_leftovers);
		void							switch_my_modes(char ope, char modeOption, std::string channel, std::string content);
		void							send_topic_updates_to_all_members_of_chan(Client *my_client, std::string channel, std::string content);

		std::map<int, std::string>&	getConcernedBuffer();
		void setClientsMap(std::map<int, std::string>& _input);

		std::vector<std::string>	getActiveChan();
		std::vector<std::string>	getNumActiveChan(long unsigned int parsing);

	private :
		typedef void (Command::*fct)(std::string leftovers);
		std::string 				command_name;
		std::string 				command_leftovers;

		Client 						*my_client;
		int							status;

		std::map<int, std::string>	Concerned_Buffers;

		int							_flagPart;
		int							_flagSpace;
		std::string					_reasonWhy;
		bool						_flagShouldCloseChan;
		bool						_flagIsThereAReason;
		std::string					_topic;

		std::map<std::string, std::string>	_chanKey;

		bool						_isGivenKeyInJoin;
		std::vector<std::string>	_numerousActiveVectorChan;
};

#endif
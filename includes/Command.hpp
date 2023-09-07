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

		std::vector<std::string> const	&getConcernedClients() const;
		std::vector<std::string> const	&getBroadCastVector() const;
		const std::string 				getResponseBuffer() const;

		const std::string 				getBroadCastBuffer() const;
		const std::string 				getErrorcode() const;
		int								getStatus(void);
		bool 							getIs_ready() const;
		bool 							getIs_Not_Accepted() const;
		const std::string				getLeftOver() const;

		void 							setIs_ready(bool _is_ready);
		void 							setConcernedClients(const Client *my_client);
		void 							setConcernedClients(const std::string _nick);
		void 							setStatus(int status);

		std::vector<std::string>		eraseUserFromChan(std::vector<std::string> memberOfThisChan, std::string userNickname, Client *my_client);

		std::map<Channel *, bool>		parsingJoin(std::string command_leftovers, std::map<Channel *, bool> chanList);
		std::string						builtReasonWhy(std::vector<std::string> reason);
		std::vector<std::string>		eraseUserFromChan(std::vector<std::string> memberOfThisChan, std::string userNickname);
		void							invite();
		void							join();
		void							kick();
		void							list();
		void							names();
		void							oper();
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
		bool 							containsOnlySpaces(const std::string& str);
		bool 							is_token_valid(std::string nick);
		int 							parseUserCommand(const std::string &input, Client *my_client);
		std::string 					extractAfterUppercase(const std::string& input);
		void 							fill_error_need_more_params(Command *my_command);
		void 							fill_error_password_mismatch(Command *my_command);
		void 							fill_error_already_registered(void);
		void							fill_no_such_nick(void);
		void 							displayChannelAndMember(std::map<Channel *, bool> chanList);
		void							send_arrived_to_all_channel(std::string nameChan, Client *my_client, std::string all_names);
		std::string						give_back_all_users_names(std::vector<std::string> members_of_chans, std::string channel_name);
		std::string						give_all_names(std::vector<std::string> members_of_chans, std::string channel_name);
		std::vector<std::string>		kick_user(std::vector<std::string> memberOfThisChan, std::string userNickname);
		void							broadcast_to_all_channels_user_is_in(Client *my_client);

		std::map<int, std::string>& getConcernedBuffer() {
			return Concerned_Buffers;
		};
		void setClientsMap(std::map<int, std::string>& _input) {
			Concerned_Buffers = _input;
		};

	private :
		typedef void (Command::*fct)(std::string leftovers);
		std::string 				command_name;
		std::string 				command_leftovers;
		std::string 				error_code;
		Client 						*my_client;
		std::vector<std::string>	concerned_clients;
		bool						is_not_accepted;
		bool						is_ready;
		int							status;
		std::string					response_buffer;
		std::string					broadcast_buffer;
		std::vector<std::string>	broadcast_vector;
		std::map<int, std::string>	Concerned_Buffers;

		int							_flagPart;
		int							_flagSpace;
		std::string					_reasonWhy;
		bool						_flagShouldCloseChan;
		bool						_flagIsThereAReason;
		std::string					_topic;

		bool						_isOperatorMode;
		bool						_isSuperOperMode;
		bool						_canalInviteOnlyMode;
		bool						_topicRestrictionMode;
		int							_limitUserMode;
		std::string					_newKeyMode;

};

#endif
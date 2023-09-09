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
		Channel *										create_new_channel_from_scratch(std::string nameChan, std::string client_nick, Client *my_client);
		std::map<std::string, std::string> 				getChanKey();
		std::map<Channel *, bool>						parsingJoin(std::string command_leftovers, std::map<Channel *, bool> chanList);
		std::vector<std::string> const					&getConcernedClients() const;
		std::vector<std::string> const					&getBroadCastVector() const;
		std::vector<std::string> 						getNumActVecChan();
		std::vector<std::string>						eraseUserFromChan(std::vector<std::string> memberOfThisChan, std::string userNickname);
		std::vector<std::string>						getActiveChan();
		std::vector<std::string>						getNumActiveChan(long unsigned int parsing);
		std::vector<std::string>						eraseUserFromChan(std::vector<std::string> memberOfThisChan, std::string userNickname, Client *my_client);
		std::vector<std::string>						kick_user(std::vector<std::string> memberOfThisChan, std::string userNickname);
		std::vector<std::string>						split_leftovers_by_comas(std::string &command_leftovers);
		const std::string 								getResponseBuffer() const;
		const std::string 								getBroadCastBuffer() const;
		const std::string								getLeftOver() const;
		const std::string 								getErrorcode() const;
		std::string										builtReasonWhy(std::vector<std::string> reason);
		std::string										give_back_all_users_names(std::vector<std::string> members_of_chans, std::string channel_name);
		std::string										give_all_names(std::vector<std::string> members_of_chans, std::string channel_name);
		std::string 									extractAfterUppercase(const std::string& input);
		int 											parseUserCommand(const std::string &input, Client *my_client);
		int												getStatus(void);
		bool 											containsOnlySpaces(const std::string& str);
		bool 											is_token_valid(std::string nick);
		bool 											getIs_Not_Accepted() const;
		bool 											getIs_ready() const;
		void											invite();
		void											join();
		void											kick();
		void											list();
		void											names();
		void											oper();
		void											part();
		void											privmsg();
		void											topic();
		void											mode();
		void											capls();
		void											user();
		void											nick();
		void											pass();
		void											ping();
		void											whois();
		void											quit();
		void											who();
		void 											setIs_ready(bool _is_ready);
		void 											setConcernedClients(const Client *my_client);
		void 											setConcernedClients(const std::string _nick);
		void 											setStatus(int status);
		void 											fill_error_need_more_params(Command *my_command);
		void 											fill_error_password_mismatch(Command *my_command);
		void 											fill_error_already_registered(void);
		void											fill_no_such_nick(void);
		void 											displayChannelAndMember(std::map<Channel *, bool> chanList);
		void											send_arrived_to_all_channel(std::string nameChan, Client *my_client, std::string all_names);
		void											broadcast_to_all_channels_user_is_in(Client *my_client);
		void											send_to_all_users_from_chan(Client *my_client, std::string nameChan, std::vector<std::string> members);
		void											switch_my_modes(char ope, char modeOption,std::map<Channel *, bool>::iterator it, std::string content);
		std::map<int, std::string>& getConcernedBuffer() {
			return Concerned_Buffers;
		};
		void setClientsMap(std::map<int, std::string>& _input) {
			Concerned_Buffers = _input;
		};

	private :
		typedef void (Command::*fct)(std::string leftovers);
		Client 											*my_client;
		std::map<int, std::string>						Concerned_Buffers;
		std::map<std::string, std::string>				_chanKey;
		std::vector<std::string>						concerned_clients;
		std::vector<std::string>						broadcast_vector;
		std::vector<std::string>						_numerousActiveVectorChan;
		std::string 									command_name;
		std::string 									command_leftovers;
		std::string 									error_code;
		std::string										response_buffer;
		std::string										broadcast_buffer;
		std::string										_reasonWhy;
		std::string										_topic;
		bool											is_not_accepted;
		bool											is_ready;
		bool											_flagShouldCloseChan;
		bool											_flagIsThereAReason;
		bool											_isGivenKeyInJoin;
		int												status;
		int												_flagPart;
		int												_flagSpace;
};

#endif
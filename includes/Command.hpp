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

		//setters et getters
		void setIs_ready(bool _is_ready);
		const std::string getResponseBuffer() const;
		bool getIs_ready() const;
		bool getIs_Not_Accepted() const;
		const std::string getErrorcode() const;
		void setConcernedClients(const Client *my_client);
		void setConcernedClients(const std::string _nick);
		void setStatus(int status);

		std::vector<std::string> const	&getConcernedClients() const;
		int								getStatus(void);

		//ZANOT
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

		//YNNAF
		void capls();
		void user();
		void nick();
		void pass();
		void ping();
		void whois();
		void quit();
		//Parsing user et nick
		bool containsOnlySpaces(const std::string& str);
		bool is_token_valid(std::string nick);
		int parseUserCommand(const std::string &input, Client *my_client);
		std::string extractAfterUppercase(const std::string& input);

		void fill_error_need_more_params(Command *my_command);
		void fill_error_password_mismatch(Command *my_command);
		void fill_error_already_registered(void);
		void displayChannelAndMember(std::map<Channel *, bool> chanList);


	private :
		typedef void (Command::*fct)(std::string leftovers);
		std::string command_name;
		std::string command_leftovers;
		std::string error_code;
		Client *my_client;
		std::vector<std::string>	concerned_clients;
		bool				is_not_accepted;
		bool				is_ready;

		int							status;
		std::string					response_buffer;
		int							_flagPart;
		int							_flagSpace;
		std::string					_reasonWhy;
		bool						_flagShouldCloseChan;
		bool						_flagIsThereAReason;
};

#endif

// 		std::vector<std::string>	concerned_clients;
// 		int							status;
// 		std::string					response_buffer;
// };

// #endif

// #ifndef COMMAND_HPP
// #define COMMAND_HPP

// #include <string>
// #include <iostream>
// #include <vector>
// #include <sys/epoll.h>
// #include <strings.h>
// #include <netdb.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <unistd.h>
// #include <fcntl.h>
// #include <cstdio>
// #include <sstream>
// #include "Server.hpp"
// #include "Client.hpp"
// #include "Channel.hpp"

// class Command : public Server
// {
// 	public :
// 		Command();
// 		~Command();
// 		Command(std::string &com, Client *_my_client);


// 		void setIs_ready(bool _is_ready);

// 		const std::string getResponseBuffer() const;
// 		bool getIs_ready() const;
// 		bool getIs_Not_Accepted() const;
// 		const std::string getErrorcode() const;
// 		bool containsOnlySpaces(const std::string& str);
// 		bool is_token_valid(std::string nick);
// 		int parseUserCommand(const std::string &input, Client *my_client);

// 		void setConcernedClients(const Client *my_client);
// 		void setStatus(int status);

// 		std::map<Channel *, bool> parsingJoin(std::string command_leftovers, std::map<Channel *, bool> chanList);
// 		std::string builtReasonWhy(std::vector<std::string> reason);

// 		std::string extractAfterUppercase(const std::string& input);
// 		//ZANOT
// 		void invite();
// 		void join();
// 		void kick();
// 		void list();
// 		void names();
// 		void oper();
// 		void part();
// 		void privmsg();
// 		void topic();
// 		void mode();

// 		//YNNAF
// 		void capls();
// 		void user();
// 		void nick();
// 		void pass();
// 		void ping();
// 		void whois();
// 		void quit();


// 	private :
// 		typedef void (Command::*fct)(std::string leftovers);
// 	// private :
// 	// 	typedef void (Command::*fct)(void);
// 		std::string response_buffer;
// 		std::string command_name;
// 		std::string command_leftovers;
// 		std::string error_code;
// 		Client *my_client;
// 		bool				is_not_accepted;
// 		bool				is_ready;
// };

// #endif


















// class Command : public Server
// {
// 	public :
// 		Command();
// 		~Command();
// 		Command(std::string &com, Client *_my_client);
// 		std::vector<std::string> const	&getConcernedClients() const;
// 		std::map<Channel *, bool>		parsingJoin(std::string command_leftovers, std::map<Channel *, bool> chanList);
// 		std::string						builtReasonWhy(std::vector<std::string> reason);
// 		std::string						extractAfterUppercase(const std::string& input);
// 		std::vector<std::string>		eraseUserFromChan(std::vector<std::string> memberOfThisChan, std::string userNickname);
// 		const							std::string getResponseBuffer() const;
// 		const							std::string getErrorcode() const;
// 		const							std::string getTopic();
// 		const							std::string getList();
// 		const							std::string getNames();
// 		bool							is_ready() const;
// 		bool							is_Not_Accepted() const;
// 		bool							containsOnlySpaces(const std::string& str);
// 		bool							is_token_valid(std::string nick);
// 		int								getStatus(void);
// 		int								parseUserCommand(const std::string &input, Client *my_client);
// 		void							setIs_ready(bool _is_ready);
// 		void 							etConcernedClients(const Client *my_client);
// 		void							setConcernedClients(const std::string _nick);
// 		void							setStatus(int status);
// 		void							fill_error_need_more_params(Command *my_command);
// 		void							fill_error_password_mismatch(Command *my_command);
// 		void							fill_error_already_registered(void);
// 		void							displayChannelAndMember(std::map<Channel *, bool> chanList);
// 		void							invite();
// 		void							join();
// 		void							kick();
// 		void							list();
// 		void							names();
// 		void							oper();
// 		void							part();
// 		void							privmsg();
// 		void							topic();
// 		void							mode();
// 		void							capls();
// 		void							user();
// 		void							nick();
// 		void							pass();
// 		void							ping();
// 		void							whois();
// 		void							quit();

// 	private :

// 		Client 							*my_client;
// 		std::vector<std::string>		concerned_clients;
// 		std::string 					command_name;
// 		std::string						command_leftovers;
// 		std::string						error_code;
// 		std::string						response_buffer;
// 		std::string						_reasonWhy;
// 		typedef void (Command::*fct)(std::string leftovers);
// 		bool							is_ready;
// 		int								status;
// 		int								_flagPart;
// 		int								_flagSpace;
// 		bool							_flagShouldCloseChan;
// 		bool							_flagIsThereAReason;
// };

// #endif


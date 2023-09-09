#ifndef SERVER_HPP
#define SERVER_HPP

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
#include <map>
#include <iostream>
#include <string>
#include <unistd.h>


#define MAX_CLIENTS 1500

#define MAX_CHANNELS 1500

#define MAX_MEMBERS_PER_CHANNEL 500

class Channel ;

class Client ;

class Command ;

class Server
{
	 public :
		typedef void (Command::*fct)(void);
		//CONSTRUCTORS ET DESTRUCTORS
		Server(int _port_number, std::string password);
		Server(void);
		Server(Server const &src);
		Server &operator=(Server const &src);
		~Server(void);

		//SETTERS AND GETTERS
			//server side
		const std::string 						&GetServerPassword() const;
		const std::string 						&GetServerName() const;
		std::map<Channel *, bool>				&GetChannelList();
		const std::vector<void (Command::*)()>	&GetFunctionTab() const;
		const std::vector<std::string>			&GetComptab() const;
			//client management
		void							SetRegisteredClient(Client *client);
		const std::vector<Client *> 	&GetConnectedClient() const;
		std::vector<Client *>			&GetRegisteredClients() ;

		//METHODS
			//init server
		bool							init_server_socket(void);
		bool							init_server_epoll(void);

			//main_loop server
		bool							loop_running_server(void);

		//CLIENTS PROCEDURES
			//Client finders
		Client							*find_my_client(int _client_fd);
		Client							*find_destination(std::string dest_nickname);
		bool							is_my_client_registered(std::string nick);
		bool							is_client_registered(Client *my_client);
			//Main client connexion procedure
		Client							*accept_new_client(int received_events_fd);
		void							process_received_request(Client *my_client, std::string converted, int i);
			//Identification client procedure
		void							add_to_registered_clients(Client *my_client);
		bool							can_client_execute_this(std::string extracted, Client *my_client);
		bool							can_client_register(Client *my_client);

		//CLEAN SERVER 
		void							clean_them_all(void);
		void							delete_client_from_vector(Client *my_client);


		//CHANNEL HANDLELING
			//kick or leave the channels
		std::vector<std::string>		kick_user(std::vector<std::string> memberOfThisChan, std::string userNickname);
		void							leave_channels_user(Client *my_client);
			//Channels finder and askers
		Channel *						send_back_channel(std::string channel_name);
		bool							is_member_of_this_chan(std::string channel_name, std::string target);
		bool							is_channel_on_server(std::string channel_name);

		//PROCEDURE DE REPONSE
		void							dont_execute_command_if_not_registered(Client *my_client);
		void							send_to_concerned_clients(Client *my_client, Command *my_command);
		bool							loop_sending_non_blocking(int socket, std::string buffer);
		
	private :
	
		int 							epoll_fd;
		int 							port_number;
		int								server_socket_fd;

		std::string 					server_name;
		std::string 					server_password;
		
		struct sockaddr_in				server_socket_addr;
		struct epoll_event 				server_socket_event;
		struct epoll_event 				received_events[MAX_CLIENTS];

		std::vector<Client*>			connected_clients;
		std::vector<Client*>			registered_clients;

		std::vector<std::string>		compTab;
		std::vector<fct> 				fctTab;

		std::map<Channel*, bool>		_channelList;

		std::map<int, std::string>		Concerned_Buffers;

		int truc;
		int truuuuuc;

};

#endif

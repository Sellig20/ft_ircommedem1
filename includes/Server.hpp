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
#include <curl/curl.h>
#include <unistd.h>
// #include "../includes/Client.hpp"
// #include "Client.hpp"

#define MAX_CLIENTS 5000

#define MAX_CHANNELS 5000

#define MAX_MEMBERS_PER_CHANNEL 500

class Channel ;

class Client ;

class Command ;

class ServerException : public std::exception
{
	public:
		explicit ServerException(const char* message) : message_(message) {}
		virtual const char* what() const throw() {
			return message_;
			}
	private:
		const char* message_;
};

class ClientNotFoundException : public ServerException {
	public:
		ClientNotFoundException(const char* message) : ServerException(message) {}
};

class ServerInitializationException : public ServerException {
	public:
		ServerInitializationException(const char* message) : ServerException(message) {}
};

class InvalidClientException : public ServerException {
	public:
		InvalidClientException(const char* message) : ServerException(message) {}
};

class CommandNotAcceptedException : public ServerException
{
	public:
		CommandNotAcceptedException(const char* message) : ServerException(message) {}
};


class Server
{
    public :
        typedef void (Command::*fct)(void);
        //CONSTRUCTORS ET DESTRUCTORS
		Server(int _port_number, std::string password);
        Server(void);
        Server(const std::string &name);
		Server(Server const &src);
		Server &operator=(Server const &src);
		~Server(void);

        //SETTERS AND GETTERS
        void SetEpollFD(int fd);
        void SetPortNumber(int port);
        void SetServerSocketFD(int fd);
        void SetServerSocketEvent(const struct epoll_event& event);
        void SetServerName(const std::string& name);
		void SetServerPassword(const std::string& name);
        const std::string& GetServerPassword() const;
        void SetServerSocketAddress(const struct sockaddr_in& addr);
        void    SetRegisteredClient(Client *client);
		void	SetLogsClient(Client *my_client);

		std::vector<Client *> &GetLogsClient(void);

        const std::string& GetServerName() const;
        const struct sockaddr_in& GetServerSocketAddress() const;
        int GetEpollFD() const;
        int GetPortNumber() const;
        int GetServerSocketFD() const;
        const struct epoll_event& GetServerSocketEvent() const;
		const std::vector<Client *> &GetConnectedClient() const;
		std::vector<Client *> &GetRegisteredClients() ;
		// void	run_the_bot(void);

        //METHODS
        bool init_server_socket(void);
        bool init_server_epoll(void);
        bool loop_running_server(void);

		//CLIENTS PROCEDURES
        Client *find_my_client(int _client_fd, int is_log);
		bool	is_my_client_registered(std::string nick);
		void	delete_client_from_vector(Client *my_client);
		void	add_to_connected_clients(Client *my_client);
		void 	add_to_registered_clients(Client *my_client);
		bool	is_client(Client *my_client);
		bool	is_client_registered(Client *my_client);

		//CLEAN SERVER 
		void	clean_them_all(void);
		
		//FULL CLIENT/SERVER PROCEDURE
		Client *accept_new_client(int received_events_fd);
		void process_received_request(Client *my_client, std::string converted, int i);
		void redaction_answer_request(Command *my_command, std::string concerned_client_nick, Client *expediteur);

		//PROCEDURE DE REPONSE
		void dont_execute_command_if_not_registered(Client *my_client);
		void send_welcome_to_client(Client *my_client, Command *my_command);
		void send_error_to_not_registered_client(Client *my_client, Command *my_command);
		Client *find_destination(std::string dest_nickname);
		void send_to_concerned_clients(Client *my_client, Command *my_command);
		bool loop_sending_non_blocking(Client *my_client);


		//PROCEDURE AUTORISATION CLIENT
		bool can_client_execute_this(std::string extracted, Client *my_client);
		bool can_client_register(Client *my_client);




		static Server& getInstance() {
        static Server instance; // Instance unique de Serveur
        return instance;
   		};
 		static void setInstance(Server& newInstance) {
        instance = &newInstance; // Définir l'instance avec la nouvelle instance fournie
    	};
        //ZANOT SPACE
        void SetComptab(std::vector<std::string> _comptab);
        void SetFunctionTab(std::vector<fct> _fctTab);
        void addChannelList(std::map<Channel*, bool> &chanList);
		void getCurlSockets(CURLM* multi_handle);

        std::map<Channel *, bool>& GetChannelList();
        const std::vector<void (Command::*)()>& GetFunctionTab() const;
        const std::vector<std::string>& GetComptab() const;
        
    private :
        static Server* instance;
        std::string server_name;
		std::string server_password;
        std::string buffer_to_send;

		std::string ip_adress;
		uint16_t			port_host;
        struct sockaddr_in server_socket_addr;
        int epoll_fd;
        int port_number;
        int server_socket_fd;
        struct epoll_event server_socket_event;
        struct epoll_event received_events[MAX_CLIENTS];
        std::vector<Client*> connected_clients;
		std::vector<Client *> logs_clients;
		std::vector<Client*> registered_clients;
        std::vector<std::string> compTab;
        // typedef void (Command::*fct)(void);
        std::vector<fct> fctTab;
		std::map<Channel*, bool> _channelList;

		//relatif au bot
		CURL *curl;
		CURLcode res;
		Client *my_bot;
		Channel *my_bot_channel;

		struct curl_slist* headers_with_content_type;
		struct curl_slist* headers;
		std::string response_data_bot;
		const char *url;
		const char *json_data;
		int socket_reception_bot;
		int socket_emission_bot;
		bool is_bot_registered;
		bool has_conversation_started_yet;
		CURLM* multi_handle;
		std::vector<int> sockets_bot;




	protected :

};

#endif

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

// #include "Client.hpp"

#define MAX_CLIENTS 10

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

        //METHODS
        bool init_server_socket(void);
        bool init_server_epoll(void);
        bool loop_running_server(void);
        Client *find_my_client(int _client_fd, int is_log);
		bool	is_already_client_ip(std::string new_ip);
		void	delete_client_from_vector(Client *my_client);
		void	add_to_connected_clients(Client *my_client);
		void 	add_to_registered_clients(Client *my_client);
		bool	is_client(Client *my_client);
		bool	is_client_registered(Client *my_client);
		void	clean_them_all(void);
		Client *accept_new_client(int received_events_fd);
		void process_received_request(Client *my_client, std::string converted, int i);
		void redaction_answer_request(Command *my_command, int i);
		Client *find_destination(std::string dest_nickname);

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

        const std::map<Channel *, bool>& GetChannelList();
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



	protected :

};

#endif

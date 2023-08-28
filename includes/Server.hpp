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

// #include "Client.hpp"

#define MAX_CLIENTS 10

class Client ;

class Command ;

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
        Client *find_my_client(int _client_fd);
		bool	is_already_client_ip(std::string new_ip);
		void	delete_client_from_vector(Client *my_client);
		void	add_to_connected_clients(Client *my_client);
		void 	add_to_registered_clients(Client *my_client);
		bool	is_client(Client *my_client);
		bool	is_client_registered(Client *my_client);
		void	clean_them_all(void);

		static Server& getInstance() {
        static Server instance; // Instance unique de Serveur
        return instance;
   		};

        //ZANOT SPACE
        void SetComptab(std::vector<std::string> _comptab);
        void SetFunctionTab( std::vector<fct> _fctTab);

        const std::vector<void (Command::*)()>& GetFunctionTab() const;
        const std::vector<std::string>& GetComptab() const;
        
    private :
        static Server* instance;
        std::string server_name;
		std::string server_password;
        std::string buffer_to_send;
        struct sockaddr_in server_socket_addr;
        int epoll_fd;
        int port_number;
        int server_socket_fd;
        struct epoll_event server_socket_event;
        struct epoll_event received_events[MAX_CLIENTS];
        std::vector<Client*> connected_clients;
		std::vector<Client*> registered_clients;
        std::vector<std::string> compTab;
        // typedef void (Command::*fct)(void);
        std::vector<fct> fctTab;


	protected :

};

#endif

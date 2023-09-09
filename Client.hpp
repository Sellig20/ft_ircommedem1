#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <iostream>
#include <vector>
// #include <sys/epoll.h>
#include <strings.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <unistd.h>
#include <fcntl.h>
#include "Server.hpp"


class Client : public Server
{
    private :
		//identificants de connexion
        struct sockaddr_in	client_socket_addr;
        int					client_socket_fd;
        struct epoll_event	client_event;
		bool				is_registered;

        std::string			nickname;
        std::string			username;
        std::string			hostname;
        std::string			servername;
        std::string			realname;

		//pointeurs vers les objets :
        Server              *my_server;

		//variables relatives a la reception et a l'envoi des reponses
		char				buffer[1024];
		std::string			_buffer;
	
	public :

		//FORME CANONIQUE
		Client(int epoll_fd, int server_socket_fd);
		Client(void);
		Client(Server *server);
		Client(Client const &src);
		Client &operator=(Client const &src);
		~Client(void);

		//SETTERS AND GETTERS

		void SetBuffer(const char *data);
		void setNickname(const std::string& newNickname);
		void setUsername(const std::string& newUsername);
		void setHostname(const std::string& newHostname);
		void setServername(const std::string& newServername);
		void setRealname(const std::string& newRealname);
		void SetMyServer(Server *my_beautiful_server);
		void setIsRegistered(const bool _is_registered);
		void SetStringBuffer(const char *data);
		

		int	GetClientSocketFD() const;
		char *GetBuffer();
		std::string GetStringBuffer() const;
		std::string getNickname() const;
		std::string getUsername() const;
		std::string getHostname() const;
		std::string getServername() const;
		std::string getRealname() const;
		Server *getMyServer() const;
		bool		getIsRegistered() const;
        // friend std::ostream& operator<<(std::ostream& os, const Client& obj);


};

#endif

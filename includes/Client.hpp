#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <iostream>
#include <vector>
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
        struct sockaddr_in	client_socket_addr;
        struct epoll_event	client_event;
        int					client_socket_fd;
        int					bytes_read;
		bool				is_registered;
		bool				password_entered;
		bool		        is_oper;
		bool		        is_mode;
        char				buffer[1024];
		uint16_t			port_host;
		std::string			ip_adress;
        std::string			nickname;
        std::string			username;
        std::string			hostname;
        std::string			servername;
        std::string			realname;
        std::string			request_code;
		std::string			response_buffer;
        std::string			_buffer;
        Server              *my_server;


    public :

    	Client(int epoll_fd, int server_socket_fd);
        Client(void);
        Client(Server *server);
		Client(Client const &src);
		Client &operator=(Client const &src);
    	bool operator==(const Client& other) const
		{
        return nickname == other.nickname;
    	};
		~Client(void);
        void        SetClientSocketAddress(const struct sockaddr_in &addr);
        void        SetClientSocketFD(int fd);
        void        SetBuffer(const char *data);
        void        SetBytesRead(int count);
        void        SetClientEvent(const struct epoll_event &event);
        void        SetStringBuffer(const char *data);
        void        setNickname(const std::string& newNickname);
        void        setUsername(const std::string& newUsername);
        void        setHostname(const std::string& newHostname);
        void        setServername(const std::string& newServername);
        void        setRealname(const std::string& newRealname);
        void        setRequestCode(const std::string code);
        void        SetMyServer(Server *my_beautiful_server);
		void        setIsRegistered(const bool _is_registered);
		void        setMyPassword(const bool _is);
		void        setIsOper(const bool _is_oper);
		bool        GetIsOper(void) const;
		bool            getMyPassword(void) const;
		Client* CloneClient(const Client& existingClient) {
    	Client* newClient = new Client(existingClient);
    	return newClient;
		};
        const struct sockaddr_in &GetClientSocketAddress() const;
        int             GetClientSocketFD() const;
        char            *GetBuffer();
        int             GetBytesRead() const;
        const struct epoll_event &GetClientEvent() const;
		bool            getIsIntroducted() const;
        std::string     GetStringBuffer() const;
        std::string     getNickname() const;
        std::string     getUsername() const;
        std::string     getHostname() const;
        std::string     getServername() const;
        std::string     getRealname() const;
        std::string     getRequestCode() const;
		std::string     getIpAdress() const;
        Server          *getMyServer() const;
		bool            getIsRegistered() const;
        friend std::ostream& operator<<(std::ostream& os, const Client& obj);
        std::string     get_command_line(void);
        void            get_first_shot(void);
        void            AddClient(void);
};

#endif

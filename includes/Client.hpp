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
		bool				password_entered;
		uint16_t			port_host;
		std::string			ip_adress;
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
        int					bytes_read;
        std::string			request_code;
		std::string			response_buffer;


    public :

		//FORME CANONIQUE
    	Client(int epoll_fd, int server_socket_fd);
        Client(void);
        Client(Server *server);
		Client(Client const &src);
		Client &operator=(Client const &src);
		//lui na rien a faire la oups
    	bool operator==(const Client& other) const
		{
        return nickname == other.nickname;
    	};
		~Client(void);

		//SETTERS AND GETTERS
        void SetClientSocketAddress(const struct sockaddr_in &addr);
        void SetClientSocketFD(int fd);
        void SetBuffer(const char *data);
        void SetBytesRead(int count);
        void SetClientEvent(const struct epoll_event &event);
        void SetStringBuffer(const char *data);
        void setNickname(const std::string& newNickname);
        void setUsername(const std::string& newUsername);
        void setHostname(const std::string& newHostname);
        void setServername(const std::string& newServername);
        void setRealname(const std::string& newRealname);
        void setRequestCode(const std::string code);
        void SetMyServer(Server *my_beautiful_server);
		void setIsRegistered(const bool _is_registered);
		void setMyPassword(const bool _is);
		bool getMyPassword(void) const;


		Client* CloneClient(const Client& existingClient) {
    	Client* newClient = new Client(existingClient); // Appel au constructeur de copie
    // Vous pourriez effectuer des modifications supplémentaires sur le nouvel objet ici
    	return newClient;
		};

        const struct sockaddr_in &GetClientSocketAddress() const;
        int GetClientSocketFD() const;
        char *GetBuffer();
        int GetBytesRead() const;
        const struct epoll_event &GetClientEvent() const;
		bool getIsIntroducted() const;
        std::string GetStringBuffer() const;
        std::string getNickname() const;
        std::string getUsername() const;
        std::string getHostname() const;
        std::string getServername() const;
        std::string getRealname() const;
        std::string getRequestCode() const;
		std::string getIpAdress() const;
        Server *getMyServer() const;
		bool getIsRegistered() const;
        friend std::ostream& operator<<(std::ostream& os, const Client& obj);




		//METHODS
        std::string get_command_line(void);
        void get_first_shot(void);

    //METHODS
        // void AddClient(const Client& client);
        void AddClient(void);
    protected :

};

#endif

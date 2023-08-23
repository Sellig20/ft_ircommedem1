#include "Server.hpp"
#include "Client.hpp"
#include <cstring>

Server::Server(void)
{

    // std::cout << "Basic Server Co/nstructor" << std::endl;
}

Server::Server(const std::string& name) : server_name(name) {
    // Constructor implementation
}

Server::Server(int _port_number)
{
    this->port_number = _port_number;
    this->epoll_fd = -1;
    this->server_socket_fd = -1;
    this->server_name = "irrealistic.expectations.irc";

    if (init_server_socket() == false)
    {
        std::cerr << "init_server_socket() failed" << std::endl;
    }
    if (init_server_epoll() == false)
    {
        std::cerr << "init_server_epoll() failed" << std::endl;
    }
    std::cout << "\xF0\x9F\x94\xA5\xF0\x9F\x94\xA5 Server has been correctly created \xF0\x9F\x94\xA5\xF0\x9F\x94\xA5" << std::endl << std::endl;
}

Server::Server(const Server &src)
{
	this->epoll_fd = src.epoll_fd;
	this->server_socket_fd = src.server_socket_fd;
	this->server_name = src.server_name;
	this->server_socket_addr = src.server_socket_addr;
	this->connected_clients = src.connected_clients;
	this->port_number = src.port_number;
	this->server_socket_event = src.server_socket_event;
	this->buffer_to_send = src.buffer_to_send;
}

Server &Server::operator=(const Server &src)
{
	if (this != &src)
	{
		this->epoll_fd = src.epoll_fd;
		this->server_socket_fd = src.server_socket_fd;
		this->server_name = src.server_name;
		this->server_socket_addr = src.server_socket_addr;
		this->connected_clients = src.connected_clients;
		this->port_number = src.port_number;
		this->server_socket_event = src.server_socket_event;
		this->buffer_to_send = src.buffer_to_send;
	}
	return *this;
}

Server::~Server()
{
        // Clean up resources here
}

//SETTERS AND GETTERS
void Server::SetServerName(const std::string& name) {
    server_name = name;
}

const std::string& Server::GetServerName() const {
    return server_name;
}

void Server::SetServerSocketAddress(const struct sockaddr_in& addr) {
    server_socket_addr = addr;
}

const struct sockaddr_in& Server::GetServerSocketAddress() const {
    return server_socket_addr;
}

void Server::SetEpollFD(int fd) {
    epoll_fd = fd;
}

int Server::GetEpollFD() const {
    return epoll_fd;
}

const std::vector<Client *> & Server::GetConnectedClient() const
{
	return connected_clients;
}

void Server::SetPortNumber(int port) {
    port_number = port;
}

int Server::GetPortNumber() const {
    return port_number;
}

void Server::SetServerSocketFD(int fd) {
    server_socket_fd = fd;
}

int Server::GetServerSocketFD() const {
    return server_socket_fd;
}

// Setter and Getter for server_socket_event
void Server::SetServerSocketEvent(const struct epoll_event& event) {
    server_socket_event = event;
}

const struct epoll_event& Server::GetServerSocketEvent() const {
    return server_socket_event;
}



//METHODS

bool Server::init_server_socket(void)
{
    int opt = 1;
	std::string logs;
    
    if ((server_socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        std::cerr << "server_socket_fd socket creation failed" << std::endl;
        return(false);
    }

    if (setsockopt(server_socket_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        std::cerr << "server_socket_fd setsockopt failed" << std::endl;

        return(false);
    }
    server_socket_addr.sin_family = AF_INET;
    server_socket_addr.sin_addr.s_addr = INADDR_ANY;
    server_socket_addr.sin_port = htons(port_number);
    if (bind(server_socket_fd, (struct sockaddr *)&server_socket_addr, sizeof(server_socket_addr)) < 0)
	{
        std::cerr << "server_socket_fd binding failed" << std::endl;
        return(false);
    }
    if (listen(server_socket_fd, 3) < 0)
	{
        std::cerr << "server_socket_fd listening failed" << std::endl;
        return(false);
    }
	return (true);
}

bool Server::init_server_epoll(void)
{
    epoll_fd = epoll_create1(0);
    if (epoll_fd == -1)
	{
        std::cerr << "epoll_fd creation failed" << std::endl;
        return(false);
    }
    server_socket_event.events = EPOLLIN;
    server_socket_event.data.fd = server_socket_fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_socket_fd, &server_socket_event) == -1) {
        std::cerr << "epoll_ctl server_socket failed" << std::endl;
        return(false);
    }
	return (true);
}

Client * Server::find_my_client(int _client_fd)
{
    for(size_t i = 0; i < connected_clients.size(); i++)
    {
        if (connected_clients[i]->GetClientSocketFD() == _client_fd)
            return (connected_clients[i]);
    }
    return (NULL);
}

bool is_client(const std::vector<Client *> vec, int value) {
    for (size_t i = 0; i < vec.size(); ++i) {
        if (vec[i]->GetClientSocketFD() == value) {
            return true;
        }
    }
    return false;
}


bool Server::loop_running_server(void)
{
	int max_events = 0;
	char buffer[1024];	
	while (1)
	{
		max_events = epoll_wait(epoll_fd, received_events, 10, -1);
        for (int i = 0; i < max_events; i++)
        {
			if (received_events[i].data.fd == server_socket_fd)
			{
				//Creation d'un nouveau client pcq socket pas encore ouverte
				Client *new_client =  new Client(epoll_fd, server_socket_fd);
				if (is_client(connected_clients, new_client->GetClientSocketFD()) == false)
				{
					std::cout << "\xF0\x9F\x86\x95 CLIENT CONNECTED \xF0\x9F\x86\x95" << std::endl;
					connected_clients.push_back(new_client);
				}
				else
				{
					std::cout << "Error occured identifying the client" << std::endl;
					delete new_client;
				}

			}
			else
			{
				//Client deja connu, protocole EPOLLIN/EPOLLOUT
				Client *my_client = find_my_client(received_events[i].data.fd);
				if (my_client == NULL)
					continue ;
                if (received_events[i].events && received_events[i].events == EPOLLIN)
                {
					//on receptionne le buffer
					memset(buffer, 0, 1024);
                    my_client->SetBytesRead(recv(received_events[i].data.fd, buffer, 1024, 0));
                    if (my_client->GetBytesRead() <= 0)
                    { 
						if (my_client->GetBytesRead() == 0)
	                        std::cerr << "\xE2\x9D\x8C CLIENT DISCONNECTED \xE2\x9D\x8C" << std::endl;
						else
							std::cerr << "client socket reception failed" << std::endl;
						epoll_ctl(epoll_fd, EPOLL_CTL_DEL, received_events[i].data.fd, NULL);
            			close(received_events[i].data.fd);
						continue ;
                    }
                    else
                    {
                        my_client->SetStringBuffer(buffer);
						if (my_client->GetStringBuffer().find("\r\n") < my_client->GetStringBuffer().size())
						{
							if (my_client->getIsIntroducted() == false)
							{
								my_client->get_first_shot();
								std::string answer = ":" + GetServerName() + " " + my_client->getRequestCode() + " " + my_client->getUsername() + " :WELCOME TO THE HOOD !\r\n";
								buffer_to_send = answer;
								received_events[i].events = EPOLLOUT;
								my_client->setIsIntroducted(true);
							}
							else
							{
								std::cout << "\xF0\x9F\x93\xA5 RECEPTION DE LA REQUETE CLIENT \xF0\x9F\x93\xA5 ["  << my_client->GetStringBuffer();
								//c'est ici que tu traites la commande en theorie, tu as le client actuel et les connected clients

								//pour renvoyer un truc au client, il faut mettre l'interrupteur d'evenement du client en EPOLLOUT :
								// received_events[i].events = EPOLLOUT;
								// le buffer de reponse appartient ici au serveur il s'appelle buffer_to_send

							}
							my_client->SetStringBuffer("");
							//on reset le StringBuffer a 0
						}
                    }
                }
                if (received_events[i].events && received_events[i].events == EPOLLOUT)
                {
					// on envoie le buffer de reponse
					int bytes_sent = send(received_events[i].data.fd, buffer_to_send.c_str(), buffer_to_send.size(), 0);
					std::cout << "\xF0\x9F\x93\xA4 ENVOIE DE LA REPONSE AU CLIENT \xF0\x9F\x93\xA4 [" << buffer_to_send;
					if (bytes_sent <= 0)
						std::cout << "send to client failed" << std::endl;
					buffer_to_send.erase();
                }
                
            }
        }
    }
}


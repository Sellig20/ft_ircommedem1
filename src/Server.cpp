#include "includes/Server.hpp"
#include "includes/Client.hpp"
#include "includes/Command.hpp"
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

    compTab.push_back("CAPLS");
    compTab.push_back("USER");
    compTab.push_back("NICK");
    compTab.push_back("PASS");
    compTab.push_back("PING");
    compTab.push_back("MODE");

    fctTab.push_back(&Command::capls);
    fctTab.push_back(&Command::user);
    fctTab.push_back(&Command::nick);
    fctTab.push_back(&Command::pass);
    fctTab.push_back(&Command::ping);
    fctTab.push_back(&Command::mode);

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

void Server::SetComptab(std::vector<std::string> _comptab)
{
    compTab = _comptab;
}

void Server::SetFunctionTab( std::vector<fct> _fctTab)
{
    fctTab = _fctTab;
}

// typedef void (Command::*fct) (void);
const std::vector<void (Command::*)()>& Server::GetFunctionTab() const
{
    return fctTab;
}

const std::vector<std::string>& Server::GetComptab() const
{
    return compTab;
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
    epoll_fd = epoll_create(MAX_CLIENTS);
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


void Server::delete_client_from_vector(Client* clientToRemove) {
    // Recherche du client dans le vecteur
    std::vector<Client*>::iterator it;
    for (it = connected_clients.begin(); it != connected_clients.end(); ++it) {
        if (*it == clientToRemove) {
            break;
        }
    }

    if (it != connected_clients.end()) {
        // Suppression du client du vecteur
        connected_clients.erase(it);

        // Suppression du client de la mémoire
        delete clientToRemove;
    }
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
					new_client->SetMyServer(this);
                    std::cout << "AFTER ADDING THE SERVER = " << new_client->getMyServer()->GetServerName() << std::endl;
                    // exit(1);
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
						delete_client_from_vector(my_client);
						continue ;
                    }
					else if (strcmp(buffer, "CAP LS\r\n") == 0 && strlen(buffer) == 8)
					{
						my_client->SetStringBuffer("");
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
								// online_clients.push_back(my_client->getUsername());
								std::string answer = ":" + GetServerName() + " " + my_client->getRequestCode() + " " + my_client->getNickname() + " :WELCOME TO THE HOOD !\r\n";
								buffer_to_send = answer;
								received_events[i].events = EPOLLOUT;
								my_client->setIsIntroducted(true);
							}
							else
							{
								// my_client->get_first_shot();
								// std::cout << "\xF0\x9F\x93\xA5 RECEPTION DE LA REQUETE CLIENT \xF0\x9F\x93\xA5 ["  << my_client->GetStringBuffer();
								//c'est ici que tu traites la commande en theorie, tu as le client actuel et les connected clients

                                // std::cout << "AFTER ADDING THE SERVER = " << my_client->getMyServer()->GetServerName() << std::endl;
                                // exit(1);
                                std::cout << "Connected client nickname is [" << my_client->getNickname() << std::endl;
                                std::string buf = my_client->GetStringBuffer();
                                std::cout << "BUFFER ================> " << buf << std::endl;
                                size_t i(0);
                                int com1(0);
                                std::string com;
                                std::string leftover;
                                while (i < buf.length() && i < buf.length())
                                {
                                    if (isupper(buf[i]))
                                    {
                                        com1 = buf.find(" ");
                                        com = buf.substr(0, com1);
                                        if (com.length() > 3 && com.length() < 8)
                                        {
                                            std::cout << "Voici la commande : " << com << std::endl;
                                            i += com.length() - 1;
                                        }
                                        else
                                            std::cerr << "Commande non-existante. Message anormal" << std::endl;
                                    }
                                    else if (buf[i] == ' ' && i + 1)
                                    {
                                        com1 = buf.find(" ");
                                        leftover = buf.substr(com1, buf.size());
                                        std::cout << "Voici le leftovers : " << leftover << std::endl;
                                        i += leftover.length();
                                    }
                                    i++;
                                }
                                // std::string com = buf.substr(0, com1);
                                Command *order = new Command(com, leftover, my_client);
                                delete order;
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


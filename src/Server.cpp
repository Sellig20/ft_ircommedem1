#include "../includes/Server.hpp"
#include "../includes/Client.hpp"
#include "../includes/Command.hpp"
#include <cstring>

Server::Server(void)
{

    // std::cout << "Basic Server Co/nstructor" << std::endl;
}

Server::Server(const std::string& name) : server_name(name) {
    // Constructor implementation
}

Server::Server(int _port_number, std::string password)
{
    this->port_number = _port_number;
	this->server_password = password;
    this->epoll_fd = -1;
    this->server_socket_fd = -1;
    this->server_name = "irrealistic.expectations.irc";

    compTab.push_back("CAPLS");
    compTab.push_back("USER");
    compTab.push_back("NICK");
    compTab.push_back("PASS");
    compTab.push_back("PING");
    compTab.push_back("MODE");
    compTab.push_back("INVITE");
    compTab.push_back("JOIN");
    compTab.push_back("KICK");
    compTab.push_back("LIST");
    compTab.push_back("NAMES");
    compTab.push_back("OPER");
    compTab.push_back("PART");
    compTab.push_back("PRIVMSG");
    compTab.push_back("TOPIC");

    fctTab.push_back(&Command::capls);
    fctTab.push_back(&Command::user);
    fctTab.push_back(&Command::nick);
    fctTab.push_back(&Command::pass);
    fctTab.push_back(&Command::ping);
    fctTab.push_back(&Command::mode);
    fctTab.push_back(&Command::invite);
    fctTab.push_back(&Command::join);
    fctTab.push_back(&Command::kick);
    fctTab.push_back(&Command::list);
    fctTab.push_back(&Command::names);
    fctTab.push_back(&Command::oper);
    fctTab.push_back(&Command::part);
    fctTab.push_back(&Command::privmsg);
    fctTab.push_back(&Command::topic);


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

	// std::cout << "DESTRUCTOR SERVOR" << std::endl;
    // for (size_t i = 0; i < connected_clients.size(); i++) {
    //     delete connected_clients[i];
    // }
    // connected_clients.clear(); // Efface le vecteur
}

void	Server::clean_them_all(void)
{
	std::cout << "DESTRUCTOR SERVOR avec connect client size = " << connected_clients.size() << std::endl;
    for (size_t i = 0; i < connected_clients.size(); i++) {
        delete connected_clients[i];
    }
	for (size_t i = 0; i < registered_clients.size(); i++) {
        delete registered_clients[i];
    }
    connected_clients.clear(); 
	registered_clients.clear();
	fctTab.clear();
	compTab.clear();
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



void Server::SetServerPassword(const std::string& name)
{
	server_password = name;
}

const std::string& Server::GetServerPassword() const
{
	return server_password;
}

std::vector<Client *> &Server::GetRegisteredClients() 
{
	return registered_clients;
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

void	Server::add_to_connected_clients(Client *my_client)
{
	connected_clients.push_back(my_client);
}

void 	Server::add_to_registered_clients(Client *my_client)
{
	registered_clients.push_back(my_client);
}


bool	Server::is_client(Client *my_client)
{
	for (std::vector<Client *>::const_iterator it = connected_clients.begin(); it != connected_clients.end(); ++it) 
	{
		if (*it == my_client)
			return true;
    }
    return false;
}

bool	Server::is_client_registered(Client *my_client)
{
	for (std::vector<Client *>::const_iterator it = registered_clients.begin(); it != registered_clients.end(); ++it) 
	{
		if (*it == my_client)
			return true;
    }
    return false;
}

bool Server::loop_running_server(void)
{
	int max_events = 0;
	bool do_we_keep_you = true;
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
				if (is_client(new_client) == false)
				{
					std::cout << "\xF0\x9F\x86\x95 CLIENT CONNECTED \xF0\x9F\x86\x95" << std::endl;
					new_client->SetMyServer(this);
					connected_clients.push_back(new_client);
                    // std::cout << "AFTER ADDING THE SERVER = " << new_client->getMyServer()->GetServerName() << std::endl;
                    // exit(1);

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
					std::cout << "received : " << buffer;
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
                    else
					{
                        my_client->SetStringBuffer(buffer);
						if (my_client->GetStringBuffer().find("\r\n") < my_client->GetStringBuffer().size())
						{
							std::string converted(buffer);
							//on convertit le buffer en string pour pas se peter le cul avec les char
							size_t pos = 0;
							//pour pouvoir gerer les cas ou on a des gros buffers avec plusieurs commandes et des buffers avec une seule commande, on boucle sur le buffer tant qu'on retrouve des \r\n
							while ((pos = converted.find("\r\n", pos)) != std::string::npos)
							{
								std::string extracted = converted.substr(0, pos);
								
								Command *my_command = new Command(extracted, my_client);
								std::cout << "BUFFER DE REPONSE = " << my_command->getResponseBuffer() << " is_ready = " << my_command->getIs_ready() << " et is_accepted = " << my_command->getIs_Not_Accepted() << std::endl;
								if (my_command->getIs_ready() == true)
								{
									//preparation du buffer de renvoie en remplissant avec le nom du serv + le buffer de reponse constitue dans la fonction de la commande
									received_events[i].events = EPOLLOUT;
									buffer_to_send = ":localhost ";
									buffer_to_send += my_command->getResponseBuffer();
									buffer_to_send += "\r\n";
									//on oublie pas le \r\n qui m'a bien pt le cucul

								}
								if (my_command->getIs_Not_Accepted() == true)
								{
									//dans le cas ou on accepte pas la commande mais quon doit quand meme renvoyer un message au client pour lui dire qu'il est rejected
									do_we_keep_you = false;
								}
								delete my_command;
								//on delete pour passer a la commande suivante
								pos += 2;
								//on avance la position dans la string pour esquiver le \r\n
								converted.erase(0, pos);
								//on reset la string en retirant la commande qu'on vient de traiter
        						pos = 0; 
								//on remet le curseur a zero pour la prochaine commande
    						}
							my_client->SetStringBuffer("");
						}
                    }
                }
                if (received_events[i].events && received_events[i].events == EPOLLOUT)
                {
					int bytes_sent = send(received_events[i].data.fd, buffer_to_send.c_str(), buffer_to_send.size(), 0);
					if (bytes_sent <= 0)
						std::cout << "send to client failed" << std::endl;
					else
						std::cout << "we sent " << bytes_sent << " bytes to the client" << std::endl;
					buffer_to_send.erase();
					if (do_we_keep_you == false)
					{
						//on supprime lutilisateur si il na pas ete accepte suite a PASS/NICK/USEr
						epoll_ctl(epoll_fd, EPOLL_CTL_DEL, received_events[i].data.fd, NULL);
            			close(received_events[i].data.fd);
						delete_client_from_vector(my_client);
						do_we_keep_you = true;
						continue ;
					}
					else
					{
						std::cout << "connected client = " << connected_clients.size() << std::endl;
					}
                }

            }
        }
    }
}


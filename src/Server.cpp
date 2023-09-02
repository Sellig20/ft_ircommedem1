#include "../includes/Server.hpp"
#include "../includes/Client.hpp"
#include "../includes/Command.hpp"
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

Server* Server::instance = 0;

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

    compTab.push_back("CAP");
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
	compTab.push_back("WHOIS");
	compTab.push_back("QUIT");


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
	fctTab.push_back(&Command::whois);
	fctTab.push_back(&Command::quit);


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
	this->instance = src.instance;
	this->server_name = src.server_name;
	this->server_password = src .server_password;
	this->buffer_to_send = src.buffer_to_send;
	this->ip_adress = src.ip_adress;
	this->port_host = src.port_host;
	this->server_socket_fd = src.server_socket_fd;
	this->epoll_fd = src.epoll_fd;
	this->connected_clients = src.connected_clients;
	this->registered_clients = src.registered_clients;
	this->compTab = src.compTab;
	this->fctTab = src.fctTab;
	for (int i = 0; i < MAX_CLIENTS; i++)
		this->received_events[i] = src.received_events[i];
}

Server &Server::operator=(const Server &src)
{
	if (this != &src)
	{
		this->instance = src.instance;
		this->server_name = src.server_name;
		this->server_password = src .server_password;
		this->buffer_to_send = src.buffer_to_send;
		this->ip_adress = src.ip_adress;
		this->port_host = src.port_host;
		this->server_socket_fd = src.server_socket_fd;
		this->epoll_fd = src.epoll_fd;
		this->connected_clients = src.connected_clients;
		this->registered_clients = src.registered_clients;
		this->compTab = src.compTab;
		this->fctTab = src.fctTab;
		for (int i = 0; i < MAX_CLIENTS; i++)
			this->received_events[i] = src.received_events[i];
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
	// for (size_t i = 0; i < registered_clients.size(); i++) {
    //     delete registered_clients[i];
    // }
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

void    Server::SetRegisteredClient(Client *client)
{
    registered_clients.push_back(client);
}

void	Server::SetLogsClient(Client *my_client)
{
	logs_clients.push_back(my_client);
}

std::vector<Client *> &Server::GetLogsClient(void)
{
	return (logs_clients);
}

const std::map<Channel *, bool>& Server::GetChannelList()
{
    return _channelList;
}

void Server::addChannelList(std::map<Channel*, bool> &chanList)
{
    _channelList = chanList;
}


//METHODS

bool Server::init_server_socket(void)
{
    int opt = 1;
	std::string logs;
	// socklen_t AddrSize = sizeof(server_socket_fd);
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

	char local_ip_str[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &(server_socket_addr.sin_addr), local_ip_str, INET_ADDRSTRLEN);
	// std::cout << "Local IP ADDRESS: " << local_ip_str << std::endl;


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

Client * Server::find_my_client(int _client_fd, int is_log)
{
    for(size_t i = 0; i < connected_clients.size(); i++)
    {
        if (connected_clients[i]->GetClientSocketFD() == _client_fd)
            return (connected_clients[i]);
    }
	if (is_log)
	{
		for(size_t i = 0; i < logs_clients.size(); i++)
    	{
        	if (logs_clients[i]->GetClientSocketFD() == _client_fd)
        	    return (logs_clients[i]);
    	}
	}
    return (NULL);
}




void Server::delete_client_from_vector(Client* clientToRemove) {
    // Recherche du client dans le vecteur
	std::string nick = clientToRemove->getNickname();
    std::vector<Client*>::iterator it;

    for (it = registered_clients.begin(); it != registered_clients.end(); ++it) {
        if (*it == clientToRemove) {
            break;
        }
    }

	if (registered_clients.begin() != registered_clients.end() && registered_clients.size() > 0)
		registered_clients.erase(it);

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
	else
		connected_clients.erase(it);
	

}

void	Server::add_to_connected_clients(Client *my_client)
{
	connected_clients.push_back(my_client);
}

void 	Server::add_to_registered_clients(Client *my_client)
{
	this->registered_clients.push_back(my_client);
}

Client * Server::find_destination(std::string dest_nickname) {
    for (size_t i = 0; i < registered_clients.size(); i++) {
				// std::cout << "Testing for Nickname = " << vec[i]->GetClientSocketFD() << std::endl;

        if (registered_clients[i]->getNickname() == dest_nickname)
		{
			return (registered_clients[i]);
        }
    }
    return NULL;
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

Client *Server::accept_new_client(int received_events_fd)
{
	Client *my_client;

	if (received_events_fd == server_socket_fd)
	{
		my_client =  new Client(epoll_fd, server_socket_fd);
		my_client->SetMyServer(this);
		connected_clients.push_back(my_client);
	}
	else
	{
		my_client  = find_my_client(received_events_fd, 0);
		if (my_client == NULL)
		{
			std::cout << "returning a null client" << std::endl;
			return (NULL);
		}
	}
	return (my_client);
}

void Server::redaction_answer_request(Command *my_command, int i)
{
	received_events[i].events = EPOLLOUT;
	// buffer_to_send = ":";
	// buffer_to_send += server_name;
	// buffer_to_send += " ";
	buffer_to_send += my_command->getResponseBuffer();
	std::cout << "ANSZER REQUEST " << buffer_to_send << std::endl;
	buffer_to_send += "\r\n";
}

// void Server::send_one_answer()

void Server::process_received_request(Client *my_client, std::string converted, int i)
{
	size_t pos = 0;
	std::string extracted;
	std::string separator;

	if (converted.find("\r\n") > converted.size() && converted.find("\n") < converted.size())
		separator = "\n";
	else if (converted.find("\r\n") < converted.size())
		separator = "\r\n";
	if (separator[0])
	{
		while (!converted.empty())
		{
			pos = converted.find(separator, pos);
			extracted = converted.substr(0, pos);
			Command *my_command = new Command(extracted, my_client);
			if (my_command->getIs_ready() == true)
			{
				redaction_answer_request(my_command, i);
			}
			if (my_client->getIsRegistered() == true && my_client->getNickname().empty() == false && !my_client->getUsername().empty() == false)
			{
				this->add_to_registered_clients(my_client);
			}
			if (received_events[i].events && received_events[i].events == EPOLLOUT)
			{
				if (!buffer_to_send.empty())
					if (send(my_client->GetClientSocketFD(), buffer_to_send.c_str(), buffer_to_send.size(), 0) <= 0)
						std::cerr << "SEND failed" << std::endl;
				buffer_to_send.erase();
				my_client->SetStringBuffer("");
				my_client->SetBuffer("");
				if (my_command->getIs_Not_Accepted() == true && my_command->getIs_ready() == true)
				{
					delete my_command;
					if (is_client_registered(my_client) == true)
					{
						close(received_events[i].data.fd);
						delete_client_from_vector(my_client);
					}
					else if (my_client->getRequestCode() == "433")
					{
						close(received_events[i].data.fd);
					}
					else if  (my_client->getRequestCode() == "464")
					{
						if (is_client_registered(my_client) == true)
							delete_client_from_vector(my_client);
					}
					else
					{
						// std::cout << "leaving but not deleting" << std::endl;
					}
					break ;
				}
			}
			delete my_command;
			converted.erase(0, pos + separator.size());
			pos = 0;
		}
	}
}


bool Server::loop_running_server(void)
{
	int max_events = 0;
	Client *my_client;

	while (1)
	{
		max_events = epoll_wait(epoll_fd, received_events, 10, -1);
		// std::cout << "max_events loop beforemake for()" << std::endl;
        for (int i = 0; i < max_events; i++)
        {
			my_client = accept_new_client(received_events[i].data.fd);
			if (my_client == NULL)
				continue ;
			if (received_events[i].events && received_events[i].events == EPOLLIN)
			{
				memset(my_client->GetBuffer(), 0, 1024);
				my_client->SetBytesRead(recv(my_client->GetClientSocketFD(), my_client->GetBuffer(), 1024, 0));
				my_client->SetStringBuffer(my_client->GetBuffer());
				std::cout << std::endl;
				std::cout << "String BUFFER = " << my_client->GetStringBuffer();
				std::cout << std::endl;
				if (my_client->GetBytesRead() <= 0)
				{
					if (my_client->GetBytesRead() == 0 && my_client->getIsRegistered() == true)
						delete_client_from_vector(my_client);
					continue ;
				}
				process_received_request(my_client, my_client->GetStringBuffer(), i);
				// std::cout << "registered clients are = " << registered_clients.size() << std::endl;
            }
        }
    }
}


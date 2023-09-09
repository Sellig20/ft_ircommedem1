#include "../includes/Server.hpp"
#include "../includes/Client.hpp"
#include "../includes/Command.hpp"
#include <cstring>
#include <sys/socket.h>
#include <cerrno>
#include <netinet/in.h>
#include <arpa/inet.h>

Server::Server(void)
{
}

Server::Server(int _port_number, std::string password)
{
	this->port_number = _port_number;
	this->server_password = password;
	this->epoll_fd = -1;
	this->server_socket_fd = -1;
	this->server_name = "irc.server.casse_les_couilles";


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
	compTab.push_back("PART");
	compTab.push_back("PRIVMSG");
	compTab.push_back("TOPIC");
	compTab.push_back("WHOIS");
	compTab.push_back("QUIT");
	compTab.push_back("WHO");


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
	fctTab.push_back(&Command::part);
	fctTab.push_back(&Command::privmsg);
	fctTab.push_back(&Command::topic);
	fctTab.push_back(&Command::whois);
	fctTab.push_back(&Command::quit);
	fctTab.push_back(&Command::who);

	if (init_server_socket() == false)
	{
		std::cerr << "init_server_socket() failed" << std::endl;
	}
	if (init_server_epoll() == false)
	{
		std::cerr << "init_server_epoll() failed" << std::endl;
	}
}

Server::Server(const Server &src)
{
	this->server_name = src.server_name;
	this->server_password = src .server_password;
	this->port_number = src.port_number;

	this->server_socket_fd = src.server_socket_fd;
	this->epoll_fd = src.epoll_fd;

	this->server_socket_addr = src.server_socket_addr;
	this->server_socket_event = src.server_socket_event;
	for (int i = 0; i < MAX_CLIENTS; i++)
		this->received_events[i] = src.received_events[i];

	this->connected_clients = src.connected_clients;
	this->registered_clients = src.registered_clients;

	this->compTab = src.compTab;
	this->fctTab = src.fctTab;

	this->_channelList = src._channelList;
	this->Concerned_Buffers = src.Concerned_Buffers;

}

Server &Server::operator=(const Server &src)
{
	if (this != &src)
	{
		this->server_name = src.server_name;
		this->server_password = src .server_password;
		this->port_number = src.port_number;

		this->server_socket_fd = src.server_socket_fd;
		this->epoll_fd = src.epoll_fd;

		this->server_socket_addr = src.server_socket_addr;
		this->server_socket_event = src.server_socket_event;
		for (int i = 0; i < MAX_CLIENTS; i++)
			this->received_events[i] = src.received_events[i];

		this->connected_clients = src.connected_clients;
		this->registered_clients = src.registered_clients;

		this->compTab = src.compTab;
		this->fctTab = src.fctTab;

		this->_channelList = src._channelList;
		this->Concerned_Buffers = src.Concerned_Buffers;
	}
	return *this;
}

Server::~Server()
{
}

void	Server::clean_them_all(void)
{
	for (size_t i = 0; i < connected_clients.size(); i++)
	{
		close(connected_clients[i]->GetClientSocketFD());
		delete connected_clients[i];
	}
	connected_clients.clear(); 
	registered_clients.clear();
	fctTab.clear();
	compTab.clear();
	close (server_socket_fd);
	close (epoll_fd);	
	if (!_channelList.empty())
	{
		for (std::map<Channel*, bool>::iterator it = _channelList.begin(); it != _channelList.end(); ++it)
			delete it->first;
		_channelList.clear();

	}
}

const std::string& Server::GetServerName() const {
    return server_name;
}


const std::vector<Client *> & Server::GetConnectedClient() const
{
	return connected_clients;
}

const std::vector<void (Command::*)()>& Server::GetFunctionTab() const
{
	return fctTab;
}

const std::vector<std::string>& Server::GetComptab() const
{
	return compTab;
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


std::map<Channel *, bool>& Server::GetChannelList()
{
	return _channelList;
}



//METHODS

bool Server::is_member_of_this_chan(std::string channel_name, std::string target)
{
	for (std::map<Channel *, bool>::iterator it = _channelList.begin(); it !=_channelList.end(); it++)
	{
		if (it->first->getNameChannel() == channel_name)
		{
			for (size_t i = 0; i < it->first->getMemberOfThisChan().size(); i++)
			{
				if (it->first->getMemberOfThisChan()[i] == target)
					return (true);
			}
		}
	}
	return (false);
}

Channel *Server::send_back_channel(std::string channel_name)
{
	if (_channelList.empty())
		return (NULL);
	for (std::map<Channel *, bool>::iterator it = _channelList.begin(); it !=_channelList.end(); it++)
	{
		if (it->first->getNameChannel() == channel_name)
			return (it->first);
	}
	return (NULL);
}

bool	Server::is_channel_on_server(std::string channel_name)
{
	for (std::map<Channel *, bool>::iterator it = _channelList.begin(); it !=_channelList.end(); it++)
	{
		if (it->first->getNameChannel() == channel_name)
			return (true);
	}
	return (false);
}


bool Server::init_server_socket(void)
{
	int opt = 1;
	std::string logs;
	if ((server_socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		std::cerr << "server_socket_fd socket creation failed" << std::endl;
		return(false);
	}
	if (fcntl(server_socket_fd, F_SETFL, O_NONBLOCK) < 0)
	{
		std::cerr << "Failed to set server_socket_fd as non-blocking" << std::endl;
		return(false);
	}
	if (setsockopt(server_socket_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
	{
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

bool	Server::init_server_epoll(void)
{
	epoll_fd = epoll_create(1);
	if (epoll_fd == -1)
	{
		std::cerr << "epoll_fd creation failed" << std::endl;
		return(false);
	}
	server_socket_event.events = EPOLLIN | EPOLLERR;
	server_socket_event.data.fd = server_socket_fd;
	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_socket_fd, &server_socket_event) == -1)
	{
		std::cerr << "epoll_ctl server_socket failed" << std::endl;
		return(false);
	}
	return (true);
}


void	Server::delete_client_from_vector(Client* clientToRemove)
{
	if (connected_clients.size() == 1)
	{
		std::cout << "ON DELETE THE ONLY PELO" << std::endl;
		connected_clients.clear();
		if (registered_clients.size() == 1)
			registered_clients.clear();
		close( clientToRemove->GetClientSocketFD());
		delete clientToRemove;
		return ;
	}
	std::vector<Client*>::iterator it;
	for (it = connected_clients.begin(); it != connected_clients.end(); ++it)
		if (*it == clientToRemove)
			break;
	if (it != connected_clients.end())
	{
		connected_clients.erase(it);
		if (is_client_registered(clientToRemove) == true)
		{
			for (it = registered_clients.begin(); it != registered_clients.end(); ++it)
			{
				if (*it == clientToRemove)
				{
					registered_clients.erase(it);
					break ;
				}
			}
		}
		std::cout << "ON DELETE THE PELO" << std::endl;
		close( clientToRemove->GetClientSocketFD());
		delete clientToRemove;
		return ;
	}
}

void 	Server::add_to_registered_clients(Client *my_client)
{
	this->registered_clients.push_back(my_client);
}

Client *	Server::find_my_client(int _client_fd)
{
	for(size_t i = 0; i < connected_clients.size(); i++)
	{
		if (connected_clients[i]->GetClientSocketFD() == _client_fd)
			return (connected_clients[i]);
	}
	return (NULL);
}

Client *	Server::find_destination(std::string dest_nickname)
{
	for (size_t i = 0; i < registered_clients.size(); i++)
	{
		if (registered_clients[i]->getNickname() == dest_nickname)
		{
			return (registered_clients[i]);
		}
	}
	return NULL;
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

bool	Server::is_my_client_registered(std::string nick)
{
	for (size_t i = 0; i < registered_clients.size(); i++) 
	{
		if (registered_clients[i]->getNickname() == nick)
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
		my_client  = find_my_client(received_events_fd);
		if (my_client == NULL)
		{
			return (NULL);
		}
	}
	return (my_client);
}


std::vector<std::string> Server::kick_user(std::vector<std::string> memberOfThisChan, std::string userNickname)
{
	if (!memberOfThisChan.empty())
	{
		for (std::vector<std::string>::iterator it = memberOfThisChan.begin(); it != memberOfThisChan.end(); it++)
		{
			if (*it == userNickname)
			{
				memberOfThisChan.erase(it);
				break ;
			}
		}
	}
	return memberOfThisChan;
}

void Server::leave_channels_user(Client *my_client)
{
	std::string nameChan, all_names;

	for (std::map<Channel *, bool>::iterator it = my_client->getMyServer()->GetChannelList().begin(); it != my_client->getMyServer()->GetChannelList().end(); it++)
	{
		nameChan = it->first->getNameChannel();
		if (my_client->getMyServer()->send_back_channel(nameChan)->getMemberOfThisChan().size() > 0)
		{
			std::vector<std::string> new_list = kick_user(my_client->getMyServer()->send_back_channel(nameChan)->getMemberOfThisChan(), my_client->getNickname());
			my_client->getMyServer()->send_back_channel(nameChan)->setMemberOfthisChan(new_list);
		}
	}
	std::map<Channel *, bool>::iterator it = my_client->getMyServer()->GetChannelList().begin();
	while (it != my_client->getMyServer()->GetChannelList().end())
	{
		if (it->first->getMemberOfThisChan().size() == 0)
		{
			delete it->first;
			my_client->getMyServer()->GetChannelList().erase(it++);
			if (it == my_client->getMyServer()->GetChannelList().end() && my_client->getMyServer()->GetChannelList().size() == 0)
			{
				my_client->getMyServer()->GetChannelList().clear();
			}
		}
		else
			++it;
	}
}

bool Server::loop_sending_non_blocking(int socket, std::string buffer)
{
	while (1)
	{
		int bytesSent = send(socket, buffer.c_str(), buffer.size(), 0);
		if (bytesSent == -1)
		{
			if (errno != EAGAIN && errno != EWOULDBLOCK)
			{
				perror("SERVER: send error :");
				return (false);
			}
		}
		else if (bytesSent == 0)
		{
			std::cerr << "NO BYTE SEND " << std::endl;
			return (false);
		}
		else
		{
			std::cout << ">>" << buffer  << std::endl;
			return (true);
		}
	}
}

void Server::dont_execute_command_if_not_registered(Client *my_client)
{
	my_client->SetStringBuffer("");
	my_client->SetBuffer("");
	std::string buffer_to_send = ":" + server_name + " 451 : You are not registered, please do.\r\n";
	if (loop_sending_non_blocking(my_client->GetClientSocketFD(), buffer_to_send) == false)
	{
		std::cerr << "error sending to client" << std::endl;
		delete_client_from_vector(my_client);
		my_client = NULL;
	}
}


void Server::send_to_concerned_clients(Client *my_client, Command *my_command)
{
	if (my_command->getConcernedBuffer().size() == 1)
	{
		std::map<int, std::string>::iterator it = my_command->getConcernedBuffer().begin();
		if (loop_sending_non_blocking(it->first, it->second) == false)
		{
			leave_channels_user(find_my_client(it->first));
			delete_client_from_vector(find_my_client(it->first));
			my_client = NULL;
		}
		if (my_client)
		{
			my_client->SetStringBuffer("");
			my_client->SetBuffer("");
		}
		return ;
	}
	for (std::map<int, std::string>::iterator it = my_command->getConcernedBuffer().begin(); it != my_command->getConcernedBuffer().end(); it++)
	{
		if (loop_sending_non_blocking(it->first, it->second) == false)
		{
			leave_channels_user(find_my_client(it->first));
			delete_client_from_vector(find_my_client(it->first));
			// my_client = NULL;
			continue ;
		}
	}
	if (my_client)
	{
			my_client->SetStringBuffer("");
			my_client->SetBuffer("");
	}
}

bool Server::can_client_execute_this(std::string extracted, Client *my_client)
{
	if (is_client_registered(my_client) == false
		&& extracted.find("PASS") > extracted.size()
		&& extracted.find("NICK") > extracted.size()
		&& extracted.find("USER") > extracted.size()
		&& extracted.find("CAP") > extracted.size())
	{
		if (extracted.size() < 5)
			return (false);
	}
	return (true);
}

bool Server::can_client_register(Client *my_client)
{
	if (my_client->getIsRegistered() == true
		&& my_client->getNickname().empty() == false
		&& my_client->getUsername().empty() == false
		&& is_my_client_registered(my_client->getNickname()) == false)
	{
		return (true);
	}
	return (false);

}

void Server::process_received_request(Client *my_client, std::string converted, int i)
{
	(void)i;
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
			std::cout << "<<" << extracted << std::endl;
			if (can_client_execute_this(extracted, my_client) ==  false)
			{
				dont_execute_command_if_not_registered(my_client);
				delete_client_from_vector(my_client);
				my_client = NULL;
				return ;
			}
			Command *my_command = new Command(extracted, my_client);
			if (can_client_register(my_client) == true)
				this->add_to_registered_clients(my_client);
			if (!extracted.find("QUIT"))
			{
				send_to_concerned_clients(my_client, my_command);
				
				delete_client_from_vector(my_client);
				delete my_command;
				return ;
			}
			if (my_command->getStatus() != NO_SEND)
			{
				send_to_concerned_clients(my_client, my_command);
				if (my_client ==  NULL)
				{
					if (my_command)
						delete my_command;
					return ;
				}
			}
			if (my_command)
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
		usleep(100);
		max_events = epoll_wait(epoll_fd, received_events, MAX_CLIENTS, -1);
		std::cerr << "max_events = [" << max_events << "] and nb_clients = [" << registered_clients.size() << "] and nb_channels = [" <<  _channelList.size() << std::endl;
		for (int i = 0; i < max_events; i++)
        {
			my_client = accept_new_client(received_events[i].data.fd);
			
			if (my_client == NULL)
				continue ;
			else if (received_events[i].events && received_events[i].events == EPOLLIN)
			{
				memset(my_client->GetBuffer(), 0, 1024);
				while (true)
				{
					int bytesRead = recv(my_client->GetClientSocketFD(), my_client->GetBuffer(), 1024, 0);
					if (bytesRead == -1)
					{
						if (errno == ECONNRESET)
						{
							leave_channels_user(my_client);
							delete_client_from_vector(my_client);
							my_client = NULL;
							break ;
						}
						if (errno != EAGAIN && errno != EWOULDBLOCK)
						{
							perror("SERVER: recv error :");
							leave_channels_user(my_client);
							delete_client_from_vector(my_client);
							my_client = NULL;
							break;
						}
					}
					else if (bytesRead == 0)
					{
						// std::cerr << "NO BYTE RECV " << std::endl;
						leave_channels_user(my_client);
						delete_client_from_vector(my_client);
						my_client = NULL;
						break  ;
					}
					else if (my_client != NULL)
					{
						my_client->SetStringBuffer(my_client->GetBuffer());
						break;
					}
				}
				if (my_client != NULL)
				{
					process_received_request(my_client, my_client->GetStringBuffer(), i);
				}
			}
			
		}
	}
}
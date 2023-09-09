#include "../includes/Client.hpp"
#include <cstring>
#include <sstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

Client::Client()
{
	// std::cout << "Client successfully Constructor and epoll_fd server from here = " << GetEpollFD() << std::endl;
}

Client::Client(int epoll_fd, int server_socket_fd)
{
	is_registered = false;
	socklen_t AdrrSize = sizeof(client_socket_addr);
	client_socket_fd = accept(server_socket_fd, (struct sockaddr*)&client_socket_addr, &AdrrSize);
	if (client_socket_fd == -1)
	{
		std::cerr << "Accepting client connection failed" << std::endl;
		return ;
	}
	fcntl(client_socket_fd, F_SETFL, O_NONBLOCK);
	client_event.events = EPOLLIN | EPOLLET;
	client_event.data.fd = client_socket_fd;
	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_socket_fd, &client_event) == -1)
	{
		std::cerr << "Error adding client socket to epoll." << std::endl;
		close(client_socket_fd);
		return ;
	}
}


Client::Client(const Client &src) : Server(src)
{
	Server::operator=(src);

	this->client_socket_addr = src.client_socket_addr;
	this->client_socket_fd = src.client_socket_fd;
	this->client_event = src.client_event;

	this->_buffer = src._buffer;

	this->client_event = src.client_event;
	this->my_server = src.my_server;
	this->is_registered = src.is_registered;

	this->nickname = src.nickname;
	this->hostname = src.hostname;
	this->servername = src.servername;
	this->realname = src.realname;
	strcpy(this->buffer, src.buffer);
}

// Copy Assignment Operator
Client &Client::operator=(const Client &src)
{
	if (this != &src)
	{
		Server::operator=(src);
		this->client_socket_addr = src.client_socket_addr;
		this->client_socket_fd = src.client_socket_fd;
		this->client_event = src.client_event;

		this->_buffer = src._buffer;

		this->client_event = src.client_event;
		this->my_server = src.my_server;
		this->is_registered = src.is_registered;

		this->nickname = src.nickname;
		this->hostname = src.hostname;
		this->servername = src.servername;
		this->realname = src.realname;
		strcpy(this->buffer, src.buffer);
	}
	return *this;
}

// Destructor
Client::~Client()
{
}

int Client::GetClientSocketFD() const {
    return client_socket_fd;
}

void Client::SetBuffer(const char *data) {
	if (!data[0])
	{
		bzero(buffer, sizeof(buffer));
	}
	else
	    snprintf(buffer, sizeof(buffer), "%s", data);
}

char *Client::GetBuffer() {
    return buffer;
}


void Client::SetStringBuffer(const char *data)
{
	if (!data[0])
	{
		_buffer.erase();
	}
	_buffer += data;
}

std::string Client::GetStringBuffer() const
{
    return _buffer;
}

void Client::setNickname(const std::string& newNickname) {
    nickname = newNickname;
}

void Client::setUsername(const std::string& newUsername) {
    username = newUsername;
}

void Client::setHostname(const std::string& newHostname) {
    hostname = newHostname;
}

void Client::setServername(const std::string& newServername) {
    servername = newServername;
}

void Client::setRealname(const std::string& newRealname) {
    realname = newRealname;
}

// Getters
std::string Client::getNickname() const {
    return nickname;
}

std::string Client::getUsername() const {
    return username;
}

std::string Client::getHostname() const {
    return hostname;
}

std::string Client::getServername() const {
    return servername;
}

std::string Client::getRealname() const {
    return realname;
}

void Client::setIsRegistered(const bool _is_registered)
{
	is_registered = _is_registered;
}

bool Client::getIsRegistered() const
{
	return is_registered;
}

void Client::SetMyServer(Server *my_beautiful_server)
{
    my_server = my_beautiful_server;
}

Server *Client::getMyServer() const
{
    return this->my_server;
}

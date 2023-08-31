#include "../includes/Client.hpp"
#include <cstring>
#include <sstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

Client::Client()
{
	std::cout << "Client successfully Constructor and epoll_fd server from here = " << GetEpollFD() << std::endl;
}

Client::Client(Server *server)
{
	(void)server;
    // std::cout << "par ici avec÷ server_name = " << server->GetServerName() << std::endl;
}

std::string Client::get_command_line(void)
{
    std::string cmd_line = _buffer.substr(0, _buffer.find("\r\n"));
    // _buffer = _buffer.substr(cmd_line.size(), _buffer.size());
    // if (cmd_line == "CAP LS")
	// {
    //     // get_command_line(_buffer);
	// }
	// std::cout << "For this command = " << cmd_line << std::endl;
    return (cmd_line);
}

std::vector<std::string> splitStringByWhitespace(const std::string& input)
{
    std::istringstream iss(input);
    std::vector<std::string> result;

    std::string word;
    while (iss >> word) {
        result.push_back(word);
    }

    return result;
}

Client::Client(int epoll_fd, int server_socket_fd)
{
	is_registered = false;
	password_entered = false;
    socklen_t AdrrSize = sizeof(client_socket_addr);
    client_socket_fd = accept(server_socket_fd, (struct sockaddr*)&client_socket_addr, &AdrrSize);
    if (client_socket_fd == -1)
    {
        // std::cerr << "Accepting client connection failed" << std::endl;
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
	if (getpeername(client_socket_fd, (struct sockaddr*)&client_socket_addr, &AdrrSize) == 0)
	{
    	char client_ip_str[INET_ADDRSTRLEN];
    	inet_ntop(AF_INET, &(client_socket_addr.sin_addr), client_ip_str, INET_ADDRSTRLEN);
		// ip_adress = client_ip_str;
		port_host = ntohs(client_socket_addr.sin_port);
    	std::stringstream ip_port_ss;
    	ip_port_ss << client_ip_str << ":" << port_host;
		ip_adress = ip_port_ss.str();

		// std::cout << "new User " << ip_adress << std::endl;
	}
	else
	{
		ip_adress = "error";
		std::cerr << "error ip addressing" << std::endl;
	}
}


Client::Client(const Client &src) : Server(src)
{
	std::cout << "constructeur copie" << std::endl;
    Server::operator=(src);
    this->client_socket_addr = src.client_socket_addr;
    this->client_socket_fd = src.client_socket_fd;
    this->_buffer = src._buffer;
    this->bytes_read = src.bytes_read;
	this->request_code = src.request_code;
    this->client_event = src.client_event;
	this->my_server = src.my_server;
	this->is_registered = src.is_registered;
	this->port_host = src.port_host;
	this->ip_adress = src.ip_adress;
	this->nickname = src.nickname;
	this->hostname = src.hostname;
	this->servername = src.servername;
	this->realname = src.realname;
	this->password_entered = src.password_entered;
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
		this->_buffer = src._buffer;
		this->bytes_read = src.bytes_read;
		this->request_code = src.request_code;
		this->client_event = src.client_event;
		this->my_server = src.my_server;
		this->is_registered = src.is_registered;
		this->port_host = src.port_host;
		this->ip_adress = src.ip_adress;
		this->nickname = src.nickname;
		this->hostname = src.hostname;
		this->servername = src.servername;
		this->realname = src.realname;
		this->password_entered = src.password_entered;
		strcpy(this->buffer, src.buffer);
	}
	return *this;
}

// Destructor
Client::~Client() {
    // Clean up resources here
}

void Client::SetClientSocketAddress(const struct sockaddr_in &addr) {
    client_socket_addr = addr;
}

const struct sockaddr_in &Client::GetClientSocketAddress() const {
    return client_socket_addr;
}

// Setter and Getter for client_socket_fd
void Client::SetClientSocketFD(int fd) {
    client_socket_fd = fd;
}

int Client::GetClientSocketFD() const {
    return client_socket_fd;
}

// Setter and Getter for buffer
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

// Setter and Getter for bytes_read
void Client::SetBytesRead(int count) {
    bytes_read = count;
}

int Client::GetBytesRead() const {
    return bytes_read;
}

// Setter and Getter for client_event
void Client::SetClientEvent(const struct epoll_event &event) {
    client_event = event;
}

const struct epoll_event &Client::GetClientEvent() const {
    return client_event;
}

void Client::SetStringBuffer(const char *data)
{
	if (!data[0])
	{
		_buffer.erase();
		//on efface le buffer si le char* est vide
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

void Client::setRequestCode(const std::string code)
{
    request_code = code;
}

std::string Client::getRequestCode() const
{
    return request_code;
}


void Client::setIsRegistered(const bool _is_registered)
{
	is_registered = _is_registered;
}

bool Client::getIsRegistered() const
{
	return is_registered;
}

std::string Client::getIpAdress() const
{
	return ip_adress;
}

void Client::SetMyServer(Server *my_beautiful_server)
{
    my_server = my_beautiful_server;
}

Server *Client::getMyServer() const
{
    return this->my_server;
}

void Client::setMyPassword(const bool _is)
{
	password_entered = _is;
}

bool Client::getMyPassword(void) const
{
	 return password_entered;
}


//METHODS

// Client::Client(Server& _server) : server(_server) {
//     // Constructor implementation
// }

// void ClientInteraction::Process() {
//     // Handle client-specific interactions using server reference
// }

void Client::AddClient() {
    std::cout << "ADD CLIENT" << std::endl;
    exit(1);
    // Client-specific processing
}

std::ostream& operator<<(std::ostream& os, const Client& obj)
{
    os << "Client => (" << obj.getNickname() << ")";
    return os;
}
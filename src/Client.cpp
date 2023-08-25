#include "includes/Client.hpp"
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
    std::cout << "par ici avec server_name = " << server->GetServerName() << std::endl;
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
std::string extractAfterUppercase(const std::string& input)
{
    std::string result;
    bool uppercaseFound = false;

    for (size_t i = 0; i < input.length(); ++i) {
        if (std::isupper(input[i])) {
            uppercaseFound = true;
        } else if (uppercaseFound && std::isspace(input[i])) {
            result = input.substr(i + 1);
			// Extract substring after uppercase characters and spaces
            break;
        }
    }

    return result;
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

void Client::get_first_shot(void)
{
    std::string cmd_line;

	// std::cout << "_buffer = " << _buffer << std::endl;
    if (!_buffer.find("CAP LS"))
    {
        cmd_line = _buffer.substr(0, _buffer.find("\r\n"));
        _buffer = _buffer.substr(cmd_line.size() + 2, _buffer.size());
		// std::cout << "CAP LS COMMAND" << std::endl;
    }
    if (!_buffer.find("NICK"))
    {
        cmd_line = _buffer.substr(0, _buffer.find("\r\n"));
        nickname = extractAfterUppercase(cmd_line);
        _buffer = _buffer.substr(cmd_line.size() + 2, _buffer.size());
		setRequestCode("001");

		// std::cout << "NICK COMMAND" << std::endl;

    }
    // else
    // {
    //     std::cerr << "\xE2\x9B\x94÷\xE2\x9B\x94\xE2\x9B\x94 BUFFER NOT FULL" << std::endl;
    //     // return ;
    // }
    if (!_buffer.find("USER"))
    {
        cmd_line = _buffer.substr(0, _buffer.find("\r\n"));
        std::string line = extractAfterUppercase(cmd_line);
        std::vector<std::string> infos = splitStringByWhitespace(line);
        if (infos.size() == 4)
        {
            username = infos[0];
			//ca marchera pas si ya un
            realname = infos[3];
			setRequestCode("001");
            // std::cout << username << " has just joined server " << servername << std::endl;
			// std::cout << "USER COMMAND" << std::endl;

        }
    }
}

Client::Client(int epoll_fd, int server_socket_fd)
{
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
	is_introducted = false;
	if (getpeername(client_socket_fd, (struct sockaddr*)&client_socket_addr, &AdrrSize) == 0)
	{
    	char client_ip_str[INET_ADDRSTRLEN];
    	inet_ntop(AF_INET, &(client_socket_addr.sin_addr), client_ip_str, INET_ADDRSTRLEN);
		ip_adress = client_ip_str;
	}
	else
	{
		ip_adress = "error";
		std::cerr << "error ip addressing" << std::endl;
	}
    struct sockaddr_in local_addr;
    socklen_t local_addr_len = sizeof(local_addr);
    if (getsockname(client_socket_fd, (struct sockaddr *)&local_addr, &local_addr_len) == 0) {
        int local_port = ntohs(local_addr.sin_port);
		std::cout << "IP ADRESS IS = " << ip_adress << " and local_port = " << local_port << std::endl;
    } else {
        // Handle error
    }
    // memset(buffer, 0, 1024);
    // bytes_read = recv(client_socket_fd, buffer, 1024, 0);
    // if (bytes_read > 0)
    // {
    //     _buffer = buffer;
    //     get_first_shot();
    //     _buffer.erase();
    //     request_code = "001";
    // }
	// else
	// 	exit(1);
}


Client::Client(const Client &src) : Server(src)
{
    Server::operator=(src);
    this->client_socket_addr = src.client_socket_addr;
    this->client_socket_fd = src.client_socket_fd;
    // this->buffer /= src.GetBuffer();
    this->bytes_read = src.bytes_read;
    this->client_event = src.client_event;
    // Perform deep copy of src's data members
}

// Copy Assignment Operator
Client &Client::operator=(const Client &src) {
    if (this != &src)
    {
        Server::operator=(src);
        this->client_socket_addr = src.client_socket_addr;
        this->client_socket_fd = src.client_socket_fd;
    // this->buffer /= src.GetBuffer();
        this->bytes_read = src.bytes_read;
        this->client_event = src.client_event;
      // Call base class assignment operator
        // Perform deep copy of src's data members
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
    snprintf(buffer, sizeof(buffer), "%s", data);
}

const char *Client::GetBuffer() const {
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

void Client::setIsIntroducted(const bool _is_intro)
{
	is_introducted = _is_intro;
}

bool Client::getIsIntroducted() const
{
	return is_introducted;
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

#include "../includes/Command.hpp"
// #include "../src/Commands/Commands.cpp"

int Command::parseUserCommand(const std::string &input, Client *my_client)
{
    std::string username, hostname, servername, realname, rest;

    std::istringstream iss(input);
    iss >> username >> hostname >> servername;
    std::getline(iss, rest, ':');
    std::getline(iss, realname);

	if (!username[0])
		return (-1);
	else if (is_token_valid(username) == false)
		return (0);
	else if (is_token_valid(hostname) == false)
		return (0);

	my_client->setUsername(username);
	my_client->setHostname(hostname);
	my_client->setRealname(realname);
	return (true);

}


void Command::user()
{
	if (command_leftovers.empty())
	{
		fill_error_need_more_params(this);
		return ;
	}
	else if (my_client->getUsername()[0])
	{
		fill_error_already_registered();
		return ;
	}
	else if (my_client->getNickname().empty())
	{
		fill_no_such_nick();
		return ;
	}
	if (parseUserCommand(command_leftovers, my_client) == true)
	{
		std::string my_buffer = ":" + my_client->getNickname() + "!" + my_client->getUsername() + "@" + my_client->getMyServer()->GetServerName() + " 001 " + my_client->getNickname() + " :WELCOME TO THE QUEENS IRC SERVERRRR\r\n";
		int socket = my_client->GetClientSocketFD();
		std::string full_buffer_client = Concerned_Buffers[socket];
		full_buffer_client.append(my_buffer);
		Concerned_Buffers[socket] = full_buffer_client;
		status = NOT_ALL_SEND;
	}
}
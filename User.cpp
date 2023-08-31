#include "../includes/Command.hpp"
// #include "../src/Commands/Commands.cpp"

int Command::parseUserCommand(const std::string &input, Client *my_client)
{
    std::string username, hostname, servername, realname, rest;

    std::istringstream iss(input);
    iss >> username >> hostname >> servername;
    std::getline(iss, rest, ':');
    std::getline(iss, realname);


	// std::cout << "from parseUserCommand : " << username << " " << hostname << " " << realname << std::endl;
	// exit(1);
	if (!username[0])
		return (-1);
	else if (is_token_valid(username) == false)
		return (0);
	else if (is_token_valid(hostname) == false)
		return (0);
	// else if (is_token_valid(servername) == false)
	// 	return (0);

	my_client->setUsername(username);
	my_client->setHostname(hostname);
	my_client->setServername(servername);
	my_client->setRealname(realname);
	return (true);
	// my_client->is

}


void Command::user()
{
	// std::cout << "from USER = " << command_leftovers << std::endl;
	if (my_client->getUsername()[0])
	{
		is_not_accepted = true;
		my_client->setRequestCode("462");
		response_buffer = "462 :" + my_client->getUsername() + " You may not reregister";
		is_ready = true;
	}
	if (parseUserCommand(command_leftovers, my_client) == true)
	{
		is_not_accepted = false;
		my_client->setRequestCode("001");
		error_code = "001";
		response_buffer = "welcome on the queens server";
		is_ready = true;
		setConcernedClients(my_client->getNickname());
		setStatus(SINGLE_SEND);
		// std::cout << "User accepted biaaaaatch" << std::endl;
	}
	else if (parseUserCommand(command_leftovers, my_client) == -1)
	{
		is_not_accepted = true;
		my_client->setRequestCode("461");
		response_buffer = "462 :" + command_name + " " + my_client->getUsername() + " You may not reregister";
		is_ready = true;
	}
	else
	{
		std::cout << "parseUserCommand not accepted" << std::endl;
	}
}
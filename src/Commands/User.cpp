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
	// my_client->setServername(servername);
	my_client->setRealname(realname);
	return (true);
	// my_client->is

}


void Command::user()
{
	// std::cout << "from USER = " << command_leftovers << std::endl;
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
		is_not_accepted = false;
		my_client->setRequestCode("001");
		error_code = "001";
		response_buffer = "welcome on the queens server";
		is_ready = true;
		setConcernedClients(my_client->getNickname());
		setStatus(SINGLE_SEND);
		// std::cout << "User accepted biaaaaatch" << std::endl;
	}
}
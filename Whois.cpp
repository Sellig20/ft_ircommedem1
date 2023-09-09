#include "../includes/Command.hpp"


void Command::whois()
{
	// std::cout << "!!!!!!!!!WHO IS = " << command_leftovers << std::endl;
	if (command_leftovers.empty())
	{
		fill_error_need_more_params(this);

		return ;
	}
	else if (is_my_client_registered(command_leftovers) == false)
	{
		fill_no_such_nick();

		return ;
	}
	int socket = my_client->GetClientSocketFD();
	std::string full_buffer_client = Concerned_Buffers[socket];
	full_buffer_client.append(":" + my_client->getMyServer()->GetServerName() + " 318 " +  my_client->getNickname() + " " + my_client->getUsername() + " " + my_client->getHostname() + " * :" + my_client->getRealname() + "\r\n");
	Concerned_Buffers[socket] = full_buffer_client;
	// is_ready = true;
	setStatus(NOT_ALL_SEND);
	return ;
}
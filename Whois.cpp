#include "../includes/Command.hpp"


void Command::whois()
{
	// std::cout << "!!!!!!!!!WHO IS = " << command_leftovers << std::endl;
	if (command_leftovers.empty())
	{
		is_not_accepted = true;
		my_client->setRequestCode("431");
		error_code = "431";
		response_buffer = ": no nickname has been given to whois command";
		is_ready = true;
		setConcernedClients(my_client->getNickname());
		setStatus(SINGLE_SEND);

		return ;
	}
	else if (is_my_client_registered(command_leftovers) == false)
	{
		is_not_accepted = true;
		my_client->setRequestCode("402");
		error_code = "402";
		response_buffer = ":" + command_leftovers + " no such nickname found on server";
		is_ready = true;
		setStatus(SINGLE_SEND);
		setConcernedClients(my_client->getNickname());

		return ;
	}
	int socket = my_client->GetClientSocketFD();
	std::string full_buffer_client = Concerned_Buffers[socket];
	full_buffer_client.append(":" + my_client->getMyServer()->GetServerName() + " 318 " +  my_client->getNickname() + " " + my_client->getUsername() + " " + my_client->getHostname() + " * :" + my_client->getRealname() + "\r\n");
	Concerned_Buffers[socket] = full_buffer_client;
	is_ready = true;
	setStatus(NOT_ALL_SEND);
	return ;
}
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
	my_client->setRequestCode("318");
	error_code = "318";
	response_buffer = my_client->getNickname() + " " + my_client->getUsername() + " " + my_client->getHostname() + " * :" + my_client->getRealname();
	setConcernedClients(my_client->getNickname());
	setStatus(SINGLE_SEND);
	is_ready = true;
	is_not_accepted = false;
	// my_client->setRequestCode("311");
	is_ready = true;
	// exit(1);
}
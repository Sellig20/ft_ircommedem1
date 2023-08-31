#include "../includes/Command.hpp"


void Command::whois()
{
	// std::cout << "!!!!!!!!!WHO IS " << std::endl;
	my_client->setRequestCode("311");
	error_code = "311";
	response_buffer = my_client->getNickname() + " " + my_client->getUsername() + " " + my_client->getHostname() + " * :" + my_client->getRealname();
	setConcernedClients(my_client->getNickname());
	setStatus(SINGLE_SEND);
	is_ready = true;
	is_not_accepted = false;
	// my_client->setRequestCode("311");
	is_ready = true;
	// exit(1);
}
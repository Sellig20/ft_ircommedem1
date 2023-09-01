#include "../includes/Command.hpp"

void Command::quit()
{
	// std::cout << "QUiIiiiiiiiit" << std::endl;
	
	is_not_accepted = true;
	my_client->setRequestCode("999");
	error_code = command_leftovers;
	response_buffer = "999: " + my_client->getNickname() + " has left the server, we forget all about him";
	is_ready = true;

	
}
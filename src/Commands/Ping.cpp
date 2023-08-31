#include "../includes/Command.hpp"

void Command::ping()
{
	is_not_accepted = false;
	my_client->setRequestCode("");
	error_code = "";
	setConcernedClients(my_client->getNickname());
	setStatus(SINGLE_SEND);
	is_ready = true;
	response_buffer = "PONG :" + command_leftovers;
	
}

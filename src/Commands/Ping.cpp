#include "../includes/Command.hpp"

void Command::ping()
{
	is_not_accepted = false;
	my_client->setRequestCode("001");
	is_ready = true;
	response_buffer = "PONG :" + command_leftovers;
	
}

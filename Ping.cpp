#include "../includes/Command.hpp"

void Command::ping()
{
	is_not_accepted = false;
	my_client->setRequestCode("001");
	response_buffer = "PONG :" + command_content;
	is_ready = true;
}

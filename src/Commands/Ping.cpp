#include "../includes/Command.hpp"

void Command::ping()
{
	if (command_leftovers.empty())
	{
		fill_error_need_more_params(this);
		return ;
	}
	int socket = my_client->GetClientSocketFD();
	std::string full_buffer_client = Concerned_Buffers[socket];
	full_buffer_client.append(":" + my_client->getNickname() + "!" + my_client->getUsername() + "@" + my_client->getMyServer()->GetServerName() + " PONG :" + command_leftovers + "\r\n");
	Concerned_Buffers[socket] = full_buffer_client;
	is_ready = true;
	setStatus(NOT_ALL_SEND);
	return ;
	
}

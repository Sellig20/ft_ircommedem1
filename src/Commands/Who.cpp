#include "../includes/Command.hpp"

void Command::who()
{
	std::istringstream ss(command_leftovers);
	std::string target;
	ss >> target;
	
	if (target[0] == '#')
	{
		int socket = my_client->GetClientSocketFD();
		std::string full_buffer_client = Concerned_Buffers[socket];
		full_buffer_client.append(":" + my_client->getMyServer()->GetServerName() + " 352 " + my_client->getNickname() + " " + target + " " + my_client->getUsername() + " " + my_client->getHostname() + " " + my_client->getMyServer()->GetServerName() + " " + my_client->getNickname() + " :" + my_client->getHostname() + "\r\n");
		full_buffer_client.append(":" + my_client->getMyServer()->GetServerName() + " 315 " + my_client->getNickname() + " " + target + " :End of WHO list\r\n");
		Concerned_Buffers[socket] = full_buffer_client;
		// is_ready = true;
		setStatus(NOT_ALL_SEND);
		return ;
	}

}
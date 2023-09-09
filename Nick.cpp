#include "../includes/Command.hpp"

/* NICK ERROR
	431 no nick given ok
	432 ok
	433 ok

	NICK REPONSE
	envoi d'un message en cas de renickname

*/

bool search_for_users_nicknames(Client *my_client, std::string new_nick)
{
	for (size_t i = 0; i < my_client->getMyServer()->GetRegisteredClients().size(); i++)
	{
		if (my_client->getMyServer()->GetRegisteredClients()[i]->getNickname().empty() == false)
			if (my_client->getMyServer()->GetRegisteredClients()[i]->getNickname() == new_nick)
				return (false);
	}
	return (true);
}


void Command::nick()
{
	if (!command_leftovers[0] || command_leftovers.size() < 2)
	{
		fill_error_need_more_params(this);
		return ;
	}
	if (is_token_valid(command_leftovers) == true)
	{
		if (is_my_client_registered(my_client->getNickname()) == true)
		{
			my_client->setNickname(command_leftovers);
			int socket = my_client->GetClientSocketFD();
			std::string full_buffer_client = Concerned_Buffers[socket];
			full_buffer_client.append(":" + my_client->getNickname() + "!" + my_client->getUsername() + "@" + my_client->getMyServer()->GetServerName() + " a new nickname has been set, you are now [" + command_leftovers + "]\r\n");
			Concerned_Buffers[socket] = full_buffer_client;
			// is_ready = true;
			setStatus(NOT_ALL_SEND);
		}
		if (search_for_users_nicknames(my_client, command_leftovers) == true)
		{
			my_client->setNickname(command_leftovers);
			my_client->setIsRegistered(true);

		}
		else
		{
			//433
			int socket = my_client->GetClientSocketFD();
			std::string full_buffer_client = Concerned_Buffers[socket];
			full_buffer_client.append(":" + my_client->getNickname() + "!" + my_client->getUsername() + "@" + my_client->getMyServer()->GetServerName() + " 433 : nickname already on use.\r\n");
			Concerned_Buffers[socket] = full_buffer_client;
			// is_ready = true;
			setStatus(NOT_ALL_SEND);
		}
	}
	else
	{
		//432
		int socket = my_client->GetClientSocketFD();
		std::string full_buffer_client = Concerned_Buffers[socket];
		full_buffer_client.append(":" + my_client->getNickname() + "!" + my_client->getUsername() + "@" + my_client->getMyServer()->GetServerName() + " 432 : nickname can only be short and alphabetical.\r\n");
		Concerned_Buffers[socket] = full_buffer_client;
		// is_ready = true;
		setStatus(NOT_ALL_SEND);
	}
}
#include "../includes/Command.hpp"
// #include "../includes/errors_rfc.hpp"

bool search_for_users_nicknames(Client *my_client, std::string new_nick)
{
	for (size_t i = 0; i < my_client->getMyServer()->GetRegisteredClients().size(); i++)
	{
		if (my_client->getMyServer()->GetRegisteredClients()[i]->getNickname() == new_nick)
			return (false);
	}
	return (true);
}


void Command::nick()
{
	if (!command_leftovers[0] || command_leftovers.size() < 2)
	{
		//rajouter au single send server
		is_not_accepted = true;
		my_client->setRequestCode("431");
		error_code = "431";
		response_buffer = "no nickname given";
		is_ready = true;
		// setConcernedClients(my_client->getNickname());
		setStatus(SINGLE_SEND);
	}
	if (is_token_valid(command_leftovers) == true)
	{
		if (search_for_users_nicknames(my_client, command_leftovers) == true)
		{
			is_not_accepted = false;
			my_client->setNickname(command_leftovers);
			my_client->setRequestCode("001");
			error_code = "001";
			response_buffer = "Nickname accepted by my fat server bitch";
			is_ready = true;
			setConcernedClients(my_client->getNickname());
			setStatus(SINGLE_SEND);
		}
		else
		{
			is_not_accepted = true;
			my_client->setRequestCode("433");
			error_code = "433";
			// command_leftovers
			response_buffer = "Nick is already in use, your new nick is now ";
			is_ready = true;
			// setConcernedClients(my_client->getNickname());
			setStatus(SINGLE_SEND);
			std::cout << "nick name already in used  => " << response_buffer << std::endl;
		}
		// std::cout
	}
	else
	{
		is_not_accepted = true;
		my_client->setRequestCode("432");
		response_buffer = command_leftovers + "Erroneus nickname";
		is_ready = true;
	}
}
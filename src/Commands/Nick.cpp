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
		fill_error_need_more_params(this);
	}
	if (is_token_valid(command_leftovers) == true)
	{
		if (is_my_client_registered(my_client->getNickname()) == true)
		{
			is_not_accepted = false;
			my_client->setRequestCode("");
			response_buffer = command_leftovers + " a new nickname as been set";
			my_client->setNickname(command_leftovers);
			is_ready = true;
			setStatus(SINGLE_SEND);
			setConcernedClients(my_client->getNickname());
			return ;
		}
		if (search_for_users_nicknames(my_client, command_leftovers) == true)
			my_client->setNickname(command_leftovers);
		else
		{
			is_not_accepted = true;
			my_client->setRequestCode("433");
			error_code = "433";
			response_buffer = ":" + command_leftovers + " Nickname already in use";
			is_ready = true;
			setStatus(SINGLE_SEND);
		}
	}
	else
	{
		is_not_accepted = true;
		my_client->setRequestCode("432");
		error_code = "432";
		response_buffer = ":" + command_leftovers + " erroneus nickname";
		is_ready = true;
		setStatus(SINGLE_SEND);
	}
}
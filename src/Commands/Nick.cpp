#include "../includes/Command.hpp"
// #include "../includes/errors_rfc.hpp"

bool search_for_users_nicknames(Client *my_client, std::string new_nick)
{
	for (size_t i = 0; i < my_client->getMyServer()->GetConnectedClient().size(); i++)
	{
		if (my_client->getMyServer()->GetConnectedClient()[i]->getNickname() == new_nick)
			return (false);
	}
	return (true);
}


void Command::nick()
{
	if (!command_content[0] || command_content.size() < 2)
	{
		is_not_accepted = true;
		my_client->setRequestCode("431");
		response_buffer = "431 :no nickname given";
		is_ready = true;
	}
	if (is_token_valid(command_content) == true)
	{
		if (search_for_users_nicknames(my_client, command_content) == true)
		{
			is_not_accepted = false;
			my_client->setNickname(command_content);
			response_buffer = "Nickname accepted by my fat server bitch";
			is_ready = true;
		}
		else
		{
			is_not_accepted = true;
			my_client->setRequestCode("433");
			response_buffer = "431 :" + command_content + " Nickname is already in use";
			is_ready = true;
		}
	}
	else
	{
		is_not_accepted = true;
		my_client->setRequestCode("432");
		response_buffer = command_content + "432 :Erroneus nickname";
		is_ready = true;
	}
}
#include "../includes/Command.hpp"

void Command::privmsg()
{
	if (command_leftovers.empty())
	{
		fill_error_need_more_params(this);
		return ;
	}
	std::string expediteur, content, dest, rest;
	std::istringstream iss(command_leftovers);
	
	iss >> dest;
	std::getline(iss, rest, ':');
	std::getline(iss, content);
	if (dest[0] == '#' && !content.empty())
	{
		for (std::map<Channel *, bool>::iterator it = my_client->getMyServer()->GetChannelList().begin(); it != my_client->getMyServer()->GetChannelList().end(); it++)
		{
			if (it->first->getNameChannel() == dest)
			{
				for (size_t i = 0; i < my_client->getMyServer()->send_back_channel(dest)->getMemberOfThisChan().size(); i++)
				{
					if (my_client->getNickname() != my_client->getMyServer()->send_back_channel(dest)->getMemberOfThisChan()[i])
					{
						std::string my_buffer = ":" + my_client->getNickname() + "!" + my_client->getUsername() + "@" + my_client->getMyServer()->GetServerName() + " PRIVMSG " + dest + " :" + content +"\r\n";
						int socket = my_client->getMyServer()->find_destination(my_client->getMyServer()->send_back_channel(dest)->getMemberOfThisChan()[i])->GetClientSocketFD();
						std::string full_buffer_client = Concerned_Buffers[socket];
						full_buffer_client.append(my_buffer);
						Concerned_Buffers[socket] = full_buffer_client;
					}
				}
				break ;
			}
		}
		if (Concerned_Buffers.size() > 0)
			status = NOT_ALL_SEND;
	}
	else
	{
	Client *destinataire = my_client->getMyServer()->find_destination(dest);
	// std::cout << "ici  dest = " << dest << " and content = " << content << std::endl;
	if (destinataire == NULL)
	{
		fill_no_such_nick();
		// std::cout << "desti is NULL" << std::endl;
	}
	else if (content.empty())
	{
		fill_error_need_more_params(this);
	}
	else
	{
		is_not_accepted = false;
		my_client->setRequestCode("");
		error_code = "";
		response_buffer = content;
		is_ready = true;
		setConcernedClients(dest);
		setStatus(SINGLE_SEND);

	}
	}

}

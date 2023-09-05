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
		std::map<Channel *, bool>::const_iterator it;
		for (it = my_client->getMyServer()->GetChannelList().begin(); it != my_client->getMyServer()->GetChannelList().end(); ++it)
		{
			if (it->first->getNameChannel() == dest)
			{
				is_not_accepted = false;
				my_client->setRequestCode("");
				error_code = "";
				response_buffer = dest + " :" + content;
				for (size_t i = 0; i < it->first->getMemberOfThisChan().size(); i++)
				{
					if (it->first->getMemberOfThisChan()[i] != my_client->getNickname())
					{
						// std::cout << "CONCERNET CLIENT = " <<  it->first->getMemberOfThisChan()[i] << std::endl;
						setConcernedClients( it->first->getMemberOfThisChan()[i]);
					}
				}
				is_ready = true;
				setStatus(SINGLE_SEND);
			}
		}
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

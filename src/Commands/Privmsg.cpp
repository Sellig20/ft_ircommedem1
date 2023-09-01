#include "../includes/Command.hpp"

void Command::privmsg()
{	
    std::string expediteur, content, dest, rest;


    std::istringstream iss(command_leftovers);
    
    iss >> dest;
    std::getline(iss, rest, ':');
	std::getline(iss, content);
	Client *destinataire = my_client->getMyServer()->find_destination(dest);
	if (destinataire == NULL)
	{
		// std::cout << "desti is NULL" << std::endl;
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

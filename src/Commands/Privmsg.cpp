#include "../includes/Command.hpp"

void Command::privmsg()
{	
    std::string expediteur, content, dest, rest;


    std::istringstream iss(command_leftovers);
    // std::string , command, recipientList, messageContent;
    
    iss >> dest;
    std::getline(iss, rest, ':');
	std::getline(iss, content);
	// std::cout << "Destinataire = " << dest << "and content = " << content << std::endl;
    
    // std::istringstream recipientStream(recipientList);
    // std::string recipient;
    // while (std::getline(recipientStream, recipient, ',')) {
    //     destinataires.push_back(recipient);
    // }

	// std::string destinataire = command_leftovers.substr(0, command_leftovers.find(' '));
	// std::string message_content =
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

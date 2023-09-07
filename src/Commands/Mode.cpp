#include "../includes/Command.hpp"

void Command::mode()
{
	// std::cout << "++++++++++++ je suis dans mode voici le leftovers : " << command_leftovers << std::endl;
	std::string cible, content, rest;


    std::istringstream iss(command_leftovers);
    
    iss >> cible >> content;

	std::cout << "Depuis mode cible = [" << cible << "] et content = [" << content << "]" << std::endl;
	if (cible.find("#") < cible.size())
	{
		std::cout << "MODE IS FOR CHANNEL" << std::endl;

	}
	else if (my_client->getMyServer()->is_my_client_registered(cible) == true && my_client->getNickname() == cible)
	{
		if (content.empty())
		{
			//on renvoie tous les modes de la target
			//
			is_not_accepted = false;
			my_client->setRequestCode("221");
			error_code = "221";
			response_buffer = command_name + " je dois rajouter les user mode ici";
			is_ready = true;
			setConcernedClients(my_client->getNickname());
			setStatus(SINGLE_SEND);
		}
		else
		{
			
		}

	}
	else if (my_client->getNickname() != cible)
	{
		is_not_accepted = true;
		my_client->setRequestCode("502");
		error_code = "502";
		response_buffer = command_name + " Cant change mode for other users";
		is_ready = true;
		setConcernedClients(my_client->getNickname());
		setStatus(SINGLE_SEND);
	}
	else
	{
		fill_no_such_nick();
	}
	// exit(1);
    // std::getline(iss, rest, ':');
	// std::getline(iss, content);
	// std::vector<Client *> gg = GetConnectedClient();
	// Client *my_c = getNickname();
	// std::cout << "?????????????????? " << my_c << std::endl;
	// for (std::vector<Client *>::iterator it = gg.begin(); it != gg.end(); it++)
	// {
	// 	std::cout << "???????? " << *it << std::endl;
	// }
	// if (command_leftovers == "+i")
	// 	std::cout << "INVISIBLE MODE ACTIVATED" << std::endl;
	// else if (command_leftovers == "+t")
	// 	std::cout << "RESTRICTION MODE ACTIVATED" << std::endl;
	// else if (command_leftovers == "+k")
	// 	std::cout << "KEY MODE ACTIVATED" << std::endl;
	// else if (command_leftovers == "+o")
	// 	std::cout << "OP MEMBER MODE ACTIVATED" << std::endl;
	// else if (command_leftovers == "+l")
	// 	std::cout << "LIMITED MODE ACTIVATED" << std::endl;

}

// void Command::mode()
// {
// 	std::cout << "++++++++++++ je suis dans mode voici le leftovers : " << command_leftovers << std::endl;
// 	std::vector<Client *> gg = GetConnectedClient();
// 	// Client *my_c = getNickname();
// 	// std::cout << "?????????????????? " << my_c << std::endl;
// 	for (std::vector<Client *>::iterator it = gg.begin(); it != gg.end(); it++)
// 	{
// 		std::cout << "???????? " << *it << std::endl;
// 	}
// 	if (command_leftovers == "+i")
// 		std::cout << "INVISIBLE MODE ACTIVATED" << std::endl;
// 	else if (command_leftovers == "+t")
// 		std::cout << "RESTRICTION MODE ACTIVATED" << std::endl;
// 	else if (command_leftovers == "+k")
// 		std::cout << "KEY MODE ACTIVATED" << std::endl;
// 	else if (command_leftovers == "+o")
// 		std::cout << "OP MEMBER MODE ACTIVATED" << std::endl;
// 	else if (command_leftovers == "+l")
// 		std::cout << "LIMITED MODE ACTIVATED" << std::endl;

// }
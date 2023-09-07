#include "../includes/Command.hpp"

void Command::mode()
{
	std::cout << "++++++++++++ je suis dans mode voici le leftovers : " << command_leftovers << std::endl;

	Server *server = my_client->getMyServer();
	std::map<Channel *, bool> &chanList = server->GetChannelList();
	std::vector<std::string> tabSeg;

	std::istringstream ss(command_leftovers);
	std::string seg;

	while (std::getline(ss, seg, ' '))
	{
		tabSeg.push_back(seg);
	}
	std::cout << "TAB SEG ====> " << tabSeg << std::endl;
	std::string channelArg = tabSeg.front();
	char ope;
	char modeOption;
	std::string arg;
	if (tabSeg.size() == 1 && tabSeg[0][0] == '#')
	{
		std::cout << "irssi en forme kokiiinou" << std::endl;
		return ;
	}
	else if (tabSeg.size() == 2 && tabSeg[0] == my_client->getNickname())
	{
		std::cout << "irsiiiii pour user" << std::endl;
		return ;
	}
	else if (tabSeg.size() > 1 && tabSeg[1].size() == 2 && (tabSeg[1][0] == '+' || tabSeg[1][0] == '-'))
	{
		std::cout << "good one moi!!!!!!!!!!" << std::endl;
		ope = tabSeg[1][0];
		modeOption = tabSeg[1][1];
		if (!tabSeg[2].empty())
			arg = tabSeg[2];
	}
	else
	{
		int socket = my_client->GetClientSocketFD();
		std::string full_buffer_client = Concerned_Buffers[socket];
		full_buffer_client.append("MODE 502 " + my_client->getNickname() + " " + channelArg + " :Unknown MODE flag \r\n");
		Concerned_Buffers[socket] = full_buffer_client;
		is_ready = true;
		setStatus(NOT_ALL_SEND);
		// std::cout << "size " << tabSeg.size() << std::endl;
		// std::cout << "size de 1 " << tabSeg[1].size() << std::endl;
		// std::cout << "+-" << tabSeg[1][0] << std::endl;
		// std::cout << "ERrreuuuuuuuuuuur MODEWWW" << std::endl;
		return ;
	}
	if (ope != '\0' && modeOption != '\0')
	{
		// std::cout << "ope = " << ope << std::endl;
		// std::cout << "modeopt = " << modeOption << std::endl;
		for (std::map<Channel *, bool>::iterator it = chanList.begin(); it != chanList.end(); it++)
		{
			if (channelArg == it->first->getNameChannel())
			{
				std::cout << "channel arg => " << channelArg << std::endl;
				switch (modeOption)
				{
					case 'i':
						if (ope == '+')
						{
							_canalInviteOnlyMode = true;
						}
						else
						{
							_canalInviteOnlyMode = false;
						}
						break ;
					case 't' :
						if (ope == '+')
						{
							_topicRestrictionMode = true;
						}
						else
						{
							_topicRestrictionMode = false;
						}
						break ;
					case 'k' :
						if (ope == '+')
						{
							_newKeyMode = arg;
						}
						else
						{
							_newKeyMode.clear();
						}
						break ;
					case 'o' :
						if (ope == '+')
						{
							//si isSuperOpe == true et bah tu peux retirer ou donner l'ope a d'autre
						}
						else
						{

						}
						break ;
					case 'l' :
						if (ope == '+' && !arg.empty())
						{
							_limitUserMode = std::atoi(arg.c_str());
						}
						else
						{
							_limitUserMode = 0;
						}
						break ;
				}
			}
		}
		int socket = my_client->GetClientSocketFD();
		std::string full_buffer_client = Concerned_Buffers[socket];
		full_buffer_client.append("MODE 403 " + my_client->getNickname() + " " + channelArg + " :No such channel \r\n");
		Concerned_Buffers[socket] = full_buffer_client;
		is_ready = true;
		setStatus(NOT_ALL_SEND);
	}










	// std::string cible, content, rest;


    // std::istringstream iss(command_leftovers);
    
    // iss >> cible >> content;

	// std::cout << "Depuis mode cible = [" << cible << "] et content = [" << content << "]" << std::endl;
	// if (cible.find("#") < cible.size())
	// {
	// 	std::cout << "MODE IS FOR CHANNEL" << std::endl;

	// }
	// else if (my_client->getMyServer()->is_my_client_registered(cible) == true && my_client->getNickname() == cible)
	// {
	// 	if (content.empty())
	// 	{
	// 		//on renvoie tous les modes de la target
	// 		//
	// 		is_not_accepted = false;
	// 		my_client->setRequestCode("221");
	// 		error_code = "221";
	// 		response_buffer = command_name + " je dois rajouter les user mode ici";
	// 		is_ready = true;
	// 		setConcernedClients(my_client->getNickname());
	// 		setStatus(SINGLE_SEND);
	// 	}
	// 	else
	// 	{
			
	// 	}

	// }
	// else if (my_client->getNickname() != cible)
	// {
	// 	is_not_accepted = true;
	// 	my_client->setRequestCode("502");
	// 	error_code = "502";
	// 	response_buffer = command_name + " Cant change mode for other users";
	// 	is_ready = true;
	// 	setConcernedClients(my_client->getNickname());
	// 	setStatus(SINGLE_SEND);
	// }
	// else
	// {
	// 	fill_no_such_nick();
	// }
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
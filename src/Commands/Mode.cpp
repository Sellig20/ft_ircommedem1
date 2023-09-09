#include "../includes/Command.hpp"

/* MODE ERROR
	403 ok
	482 ok
	MODE ANSER
		324 ok
*/
std::string set_all_modes(Channel *chan)
{
	std::string modes = "+n";
	if (chan->getCanalInviteOnlyMode() == true)
		modes += "+i";
	else
		modes += "-i";
	if (chan->getTopicRestrictionMode() == true)
		modes += "+t";
	else
		modes += "-t";
	if (chan->getPassWord().empty())
		modes += "-k";
	else
		modes += "+k";
	if (chan->getLimitUserMode() == 0)
		modes += "-l";
	else
		modes += "+l";
	// std::cout << "modes = " << modes << std::endl;
	return modes;
}

void delete_from_operator(std::vector<std::string> members, std::string nick)
{
	for (size_t i = 0; i < members.size(); i++)
	{
		if (members[i] == nick)
		{
			members[i].erase(i);
			return ;
		}
	}
}

void Command::switch_my_modes(char ope, char modeOption, std::string channel, std::string content)
{
	switch (modeOption)
	{
		case 'i':
			if (ope == '+')
		{
			my_client->getMyServer()->send_back_channel(channel)->setCanalInviteOnlyMode(true);
		}
		else
		{
			my_client->getMyServer()->send_back_channel(channel)->setCanalInviteOnlyMode(false);
			
			// setStatus(NOT_ALL_SEND);
		}
		return ;
	case 't' :
		if (ope == '+')
		{
			my_client->getMyServer()->send_back_channel(channel)->setTopicRestrictionMode(true);
			
		}
		else
		{
			my_client->getMyServer()->send_back_channel(channel)->setTopicRestrictionMode(false);
			
		}
		return ;
	case 'k' :
		if (ope == '+')
		{
			my_client->getMyServer()->send_back_channel(channel)->setPassWord(content);
			my_client->getMyServer()->send_back_channel(channel)->setHasPassword(true);
			
		}
		else
		{
			my_client->getMyServer()->send_back_channel(channel)->setHasPassword(false);
			
		}
		return ;
	case 'o' :
		if (ope == '+')
		{
			// std::cout << "is client register = " << my_client->getMyServer()->is_my_client_registered(content) << " and is member of this chan = " << my_client->getMyServer()->is_member_of_this_chan(channel, content) << " and is oper" << my_client->getMyServer()->send_back_channel(channel)->is_member_operator(content) << std::endl;
			if (my_client->getMyServer()->is_my_client_registered(content) == true
				&& my_client->getMyServer()->is_member_of_this_chan(channel, content) == true
				&& my_client->getMyServer()->send_back_channel(channel)->is_member_operator(content) ==  false)
			{
				// std::cout << "added " << content << " as operator" << std::endl;
				my_client->getMyServer()->send_back_channel(channel)->addOperatorsMembers(content);
				
			}
			//si isSuperOpe == true et bah tu peux retirer ou donner l'ope a d'autre
		}
		else if (ope == '-')
		{
			if (my_client->getMyServer()->send_back_channel(channel)->is_member_operator(content) == true)
			{
				// my_client->getMyServer()->send_back_channel(channel)->getMemberOfThisChan();
				delete_from_operator(my_client->getMyServer()->send_back_channel(channel)->getMemberOfThisChan(), content);
			}
			
		}
		return ;
	case 'l' :
		if (ope == '+' && !content.empty())
		{
			my_client->getMyServer()->send_back_channel(channel)->setLimitUserMode(std::atoi(content.c_str()));
		}
		else
		{
			my_client->getMyServer()->send_back_channel(channel)->setLimitUserMode(0);
		}
		return ;
	}
}

void Command::mode()
{
	
	// std::cout << "++++++++++++ je suis dans mode voici le leftovers : " << command_leftovers << std::endl;
	std::vector<std::string> tabSeg = split_leftovers_by_comas(command_leftovers);
	std::string new_string = command_leftovers.substr(0, command_leftovers.find(" "));
	std::string content;
	if ( new_string.find("+") < new_string.size() || new_string.find("-") < new_string.size())
	{
		if (new_string != command_leftovers)
			content = command_leftovers.substr(command_leftovers.find(" ") + 1, command_leftovers.size());
	}
	else
		content = new_string;
	char ope = 0;
	char modeOption = 0;
	size_t k = 0;

	std::cout << "leaving with size = " << tabSeg.size() << " =>" << tabSeg << " and new_string = " << new_string << " and content = " << content << std::endl;

	if (tabSeg[0][0] != '#' && tabSeg.size() == 1)
	{
		//we avoid the random mode user +b
		return ;
	}
	if (new_string == content && tabSeg.size() == 1)
	{
		if (my_client->getMyServer()->is_channel_on_server(tabSeg[0]) == true)
		{
			//324
			int socket = my_client->GetClientSocketFD();
			std::string full_buffer_client = Concerned_Buffers[socket];
			full_buffer_client.append(":" + my_client->getMyServer()->GetServerName() + " 324 " + my_client->getNickname() + " " + tabSeg[0] + " " + set_all_modes(my_client->getMyServer()->send_back_channel(tabSeg[0])) + "\r\n");
			Concerned_Buffers[socket] = full_buffer_client;
			// is_ready = true;
			setStatus(NOT_ALL_SEND);
			return ;
		}
		else
		{
			return ;
		}
	}
	else if (new_string != content)
	{
		for (size_t i = 0; i < tabSeg.size(); i++)
		{
			// std::cout << "tabseg de i = " << tabSeg[i] << std::endl;
			if (my_client->getMyServer()->is_channel_on_server(tabSeg[i]) == true)
			{
				if (my_client->getMyServer()->send_back_channel(tabSeg[i])->is_member_operator(my_client->getNickname()) ==  true)
				{
					//324
					for (;k < new_string.size();)
					{
						if (new_string != content)
						{
							if (new_string[k] == '-' || new_string[k] == '+')
								ope = new_string[k];
							if (std::isalpha(new_string[++k]))
								modeOption = new_string[k];
						}
						// std::cout << "ope = " << ope << " and modeOption = " << modeOption << std::endl;
						switch_my_modes(ope, modeOption, tabSeg[i], content);
					}
					int socket = my_client->GetClientSocketFD();
					std::string full_buffer_client = Concerned_Buffers[socket];
					full_buffer_client.append(":" + my_client->getMyServer()->GetServerName() + " 324 " + my_client->getNickname() + " " + tabSeg[i] + " " + set_all_modes(my_client->getMyServer()->send_back_channel(tabSeg[i])) + "\r\n");
					Concerned_Buffers[socket] = full_buffer_client;
					// is_ready = true;
					setStatus(NOT_ALL_SEND);
					if (tabSeg.size() == 1)
						break ;
				}
				else
				{
				//482
				int socket = my_client->GetClientSocketFD();
				std::string full_buffer_client = Concerned_Buffers[socket];
				full_buffer_client.append(":" + tabSeg[i] + "@"  + my_client->getMyServer()->GetServerName() + " 482 " + tabSeg[i] + " :you are not channel operator\r\n");
				Concerned_Buffers[socket] = full_buffer_client;
				// is_ready = true;
				setStatus(NOT_ALL_SEND);
				}
			}
			else
			{
				//403
				int socket = my_client->GetClientSocketFD();
				std::string full_buffer_client = Concerned_Buffers[socket];
				full_buffer_client.append(":" + my_client->getNickname() + "@"  + my_client->getMyServer()->GetServerName() + " 403 " + my_client->getNickname() + " :" + tabSeg[i] + "\r\n");
				Concerned_Buffers[socket] = full_buffer_client;
				// is_ready = true;
				setStatus(NOT_ALL_SEND);
			}
		}
	}
}

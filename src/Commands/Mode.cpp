#include "../includes/Command.hpp"


std::string set_all_modes(Channel *chan)
{
	std::string modes;
	if (chan->getCanalInviteOnlyMode() == true)
		modes = "+i";
	else
		modes = "-i";
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
	std::cout << "modes = " << modes << std::endl;
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

void Command::switch_my_modes(char ope, char modeOption, std::map<Channel *, bool>::iterator it, std::string content)
{
	switch (modeOption)
	{
		case 'i':
			if (ope == '+')
		{
			it->first->setCanalInviteOnlyMode(true);
		}
		else
		{
			it->first->setCanalInviteOnlyMode(false);
			
			setStatus(NOT_ALL_SEND);
		}
		return ;
	case 't' :
		if (ope == '+')
		{
			it->first->setTopicRestrictionMode(true);
			
		}
		else
		{
			it->first->setTopicRestrictionMode(false);
			
		}
		return ;
	case 'k' :
		if (ope == '+')
		{
			it->first->setPassWord(content);
			it->first->setHasPassword(true);
			
		}
		else
		{
			it->first->setHasPassword(false);
			
		}
		return ;
	case 'o' :
		if (ope == '+')
		{
			if (my_client->getMyServer()->is_my_client_registered(content) == true
				// && my_client->getMyServer()->is_member_of_this_chan(tabSeg[0], content) == true
				&& it->first->is_member_operator(content) ==  false)
			{
				std::cout << "added " << content << " as operator" << std::endl;
				it->first->addOperatorsMembers(content);
				
			}
			//si isSuperOpe == true et bah tu peux retirer ou donner l'ope a d'autre
		}
		else if (ope == '-')
		{
			if (it->first->is_member_operator(content) == true)
			{
				delete_from_operator(it->first->getOperatorsMembers(), content);
			}
			
		}
		return ;
	case 'l' :
		if (ope == '+' && !content.empty())
		{
			it->first->setLimitUserMode(std::atoi(content.c_str()));
		}
		else
		{
			it->first->setLimitUserMode(0);
		}
		return ;
	}
}

void Command::mode()
{
	std::cout << "++++++++++++ je suis dans mode voici le leftovers : " << command_leftovers << std::endl;
	Server *server = my_client->getMyServer();
	std::map<Channel *, bool> &chanList = server->GetChannelList();
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
	if (new_string != content)
	{
		for (std::map<Channel *, bool>::iterator it = chanList.begin(); it != chanList.end(); it++)
		{
			k = 0;
			for (size_t i = 0; i < tabSeg.size(); i++)
			{
				if (tabSeg[i] == it->first->getNameChannel())
				{
					for (;k < new_string.size();)
					{
						if (new_string != content)
						{
							if (new_string[k] == '-' || new_string[k] == '+')
								ope = new_string[k];
							if (std::isalpha(new_string[++k]))
								modeOption = new_string[k];
						}
						switch_my_modes(ope, modeOption, it, content);
					}
					int socket = my_client->GetClientSocketFD();
					std::string full_buffer_client = Concerned_Buffers[socket];
					full_buffer_client.append(":" + my_client->getMyServer()->GetServerName() + " 324 " + my_client->getNickname() + " " + tabSeg[i] + " " + set_all_modes(it->first) + "\r\n");
					Concerned_Buffers[socket] = full_buffer_client;
					is_ready = true;
					setStatus(NOT_ALL_SEND);
				}
			}
		}
	}
}
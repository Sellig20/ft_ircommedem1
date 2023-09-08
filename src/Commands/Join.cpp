#include "../includes/Command.hpp"
#include "../includes/Channel.hpp"
#include <algorithm>
#include <string>
#include <cerrno>


Channel *Command::create_new_channel_from_scratch(std::string nameChan, std::string client_nick, Client *my_client)
{
	std::string newbie_buffer, full_buffer_client;

	Channel *channelise = new Channel();
	channelise->setNameChannel(nameChan);
	channelise->addOperatorsMembers(client_nick);
	channelise->addMember(client_nick);
	newbie_buffer = ":" + my_client->getNickname() + "!" + my_client->getUsername() + "@" + my_client->getMyServer()->GetServerName() + " JOIN :" + nameChan + "\r\n";
	newbie_buffer.append(":" + my_client->getMyServer()->GetServerName() + " 353 " + my_client->getNickname() + " = " + nameChan + " :" + client_nick + "\r\n");
	newbie_buffer.append(":" + my_client->getMyServer()->GetServerName() + " 366 " + my_client->getNickname() + " " + nameChan + " :End of /NAMES list.\r\n");
	int socket = my_client->GetClientSocketFD();
	full_buffer_client = Concerned_Buffers[socket];
	full_buffer_client.append(newbie_buffer);
	this->Concerned_Buffers[socket] = full_buffer_client;
	return channelise;
}

std::map<std::string,std::string> separateChanAndKey(std::map<std::string,std::string> chanKey, std::string command_leftovers)
{
	std::vector<std::string> tabSpace;
	std::vector<std::string> vectorChan;
	std::vector<std::string> vectorKey;
	std::istringstream ss(command_leftovers);
	std::string seg;
	
	while (std::getline(ss, seg, ' '))
		tabSpace.push_back(seg);
	if (tabSpace.size() != 2)
	{
		std::istringstream ss1(tabSpace[0]);
		std::string seg1;
		while (std::getline(ss1, seg1, ','))
			vectorChan.push_back(seg1);
		size_t j = 0;
		while (j < vectorChan.size())
		{
			chanKey[vectorChan[j]] = "";
			j++;
		}
		return chanKey;
	}
	std::istringstream ss1(tabSpace[0]);
	std::string seg1;
	while (std::getline(ss1, seg1, ','))
		vectorChan.push_back(seg1);
	std::istringstream ss2(tabSpace[1]);
	std::string seg2;
	while (std::getline(ss2, seg2, ','))
		vectorKey.push_back(seg2);
	if (vectorChan.size() != vectorKey.size())
	{
		std::cout << "Erreur size join keychan ne correpsondent pas";
		exit(1) ;
	}
	size_t i = 0;
	size_t j = 0;
	while (i < vectorChan.size() && j < vectorKey.size())
	{
		chanKey[vectorChan[i]] = vectorKey[j];
		i++;
		j++;
	}
	return chanKey;
}


void		Command::join()
{
	size_t i = 0;
	Channel *tmp;
	std::string channel, password, seg;

	if (command_leftovers.empty())
	{
		fill_error_need_more_params(this);
		return ;
	}
	std::map<Channel*, bool>::iterator it;
	status = NOT_ALL_SEND;
	std::map<std::string, std::string> chanKey;
	chanKey = getChanKey();
	chanKey = separateChanAndKey(chanKey, command_leftovers);

	for (std::map<std::string, std::string>::iterator ita = chanKey.begin(); ita != chanKey.end(); ita++)
	{
		channel = ita->first;
		password = ita->second;
		for (it = my_client->getMyServer()->GetChannelList().begin(); it !=  my_client->getMyServer()->GetChannelList().end(); it++)
		{
			if (channel == it->first->getNameChannel() && it->first->getMemberOfThisChan().size() < MAX_MEMBERS_PER_CHANNEL
				&& my_client->getMyServer()->is_member_of_this_chan(channel, my_client->getNickname()) == false) //si tu trouves la channel, juste ajouter le client dedans
			{
				if (it->first->getHasPassword() == true && it->first->getPassWord() == password)
					it->first->addMember(my_client->getNickname());
				else if (it->first->getHasPassword() == true)
				{
					std::string newbie_buffer = ":" + my_client->getNickname() + "!" + my_client->getUsername() + "@" + my_client->getMyServer()->GetServerName() + " JOIN :" + channel + "\r\n";
					newbie_buffer.append(":" + my_client->getNickname() + "!" + my_client->getUsername() + "@" + my_client->getMyServer()->GetServerName() + " PART " + channel + " " + channel + "\r\n");
					int socket = my_client->GetClientSocketFD();
					std::string full_buffer_client = Concerned_Buffers[socket];
					full_buffer_client.append(newbie_buffer);
					this->Concerned_Buffers[socket] = full_buffer_client;
					break ;
				}
				else if (it->first->getCanalInviteOnlyMode() == true)
				{
					if (it->first->is_member_invited(my_client->getNickname()) ==  true)
					{
						my_client->getMyServer()->send_back_channel(channel)->addMember(my_client->getNickname());
					}
					else
					{
						std::string newbie_buffer = ":" + my_client->getNickname() + "!" + my_client->getUsername() + "@" + my_client->getMyServer()->GetServerName() + " JOIN :" + channel + "\r\n";
						newbie_buffer.append(":" + my_client->getNickname() + "!" + my_client->getUsername() + "@" + my_client->getMyServer()->GetServerName() + " PART " + channel + " " + channel + "\r\n");
						int socket = my_client->GetClientSocketFD();
						std::string full_buffer_client = Concerned_Buffers[socket];
						full_buffer_client.append(newbie_buffer);
						this->Concerned_Buffers[socket] = full_buffer_client;
						break ;
					}
				}
				else if (it->first->getLimitUserMode() > 0 && it->first->getMemberOfThisChan().size() < it->first->getLimitUserMode())
					my_client->getMyServer()->send_back_channel(channel)->addMember(my_client->getNickname());
				else if (it->first->getLimitUserMode() == 0)
					my_client->getMyServer()->send_back_channel(channel)->addMember(my_client->getNickname());
				else 
					my_client->getMyServer()->send_back_channel(channel)->addMember(my_client->getNickname());				
				send_to_all_users_from_chan(my_client, channel, my_client->getMyServer()->send_back_channel(channel)->getMemberOfThisChan());
				break ;
			}
			else if (channel == it->first->getNameChannel() && it->first->getMemberOfThisChan().size() == MAX_MEMBERS_PER_CHANNEL)
			{
				break ;
			}
		}
		if (it == my_client->getMyServer()->GetChannelList().end() && my_client->getMyServer()->GetChannelList().size() < MAX_CHANNELS) //sinon creation de la channel et ajout du client dedans
		{
			tmp = create_new_channel_from_scratch(channel, my_client->getNickname(), my_client);
			my_client->getMyServer()->GetChannelList().insert(std::make_pair(tmp, true));
		}
		i++;
	}
}

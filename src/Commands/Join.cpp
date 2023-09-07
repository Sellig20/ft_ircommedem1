#include "../includes/Command.hpp"
#include "../includes/Channel.hpp"
#include <algorithm>
#include <string>
#include <cerrno>

/**
 * @brief modifications fanny :
 *			- break dans parsingJoin() pour bug multiplication des channels
 *			- ajout de give_back_all_users_name pour le cas ou ya plusieurs utilisateurs dans le channel, histoire d'avoir tous les noms des users de ce channel lors de la connexion du nouvel user
 *			- envoie d'un message general a tous les utilisateurs du channel en cas de nouvelle connexion
 */
std::string Command::give_back_all_users_names(std::vector<std::string> members_of_chans, std::string channel_name)
{
	std::string all_names;
	(void)channel_name;
	for (size_t i = 0; i < members_of_chans.size(); i++)
	{
		if (i < members_of_chans.size() - 1)
			all_names.append(members_of_chans[i] + " ");
		else
			all_names.append(members_of_chans[i]);
	}
	return all_names;
}

void appendStringToMapKey(std::map<int, std::string>& myMap, int key, const std::string& content)
{
	std::map<int, std::string>::iterator it = myMap.find(key);
	
	if (it != myMap.end())
	{
	it->second += content;
	}
	else
	{
		myMap[key] = content;
	}
}

void Command::join()
{
	std::string backToBuf;
    if (!command_leftovers.empty())
	{
		Server *server = my_client->getMyServer();
		std::map<Channel *, bool> chanList = server->GetChannelList();

		chanList = parsingJoin(command_leftovers, chanList);

		std::vector<std::string> tabSeg;
		std::istringstream ss(command_leftovers);
		std::string seg;
		while (std::getline(ss, seg, ','))
		{
			tabSeg.push_back(seg);
		}
		std::string nameChan;
		std::string all_names;
		size_t i = 0;
		broadcast_buffer.erase();
		for (std::map<Channel *, bool>::iterator it = chanList.begin(); it != chanList.end(); it++)
		{
			nameChan = it->first->getNameChannel();
			if (i < tabSeg.size() && nameChan == tabSeg[i])
			{
				all_names = give_back_all_users_names(it->first->getMemberOfThisChan(), nameChan);
				std::string newbie_buffer = ":" + my_client->getNickname() + "!" + my_client->getUsername() + "@" + my_client->getMyServer()->GetServerName() + " JOIN :" + nameChan + "\r\n";
				newbie_buffer.append(":" + my_client->getMyServer()->GetServerName() + " 353 " + my_client->getNickname() + " = " + nameChan + " :" + all_names + "\r\n");
				newbie_buffer.append(":" + my_client->getMyServer()->GetServerName() + " 366 " + my_client->getNickname() + " " + nameChan + " :End of /NAMES list.\r\n");
				int socket = my_client->GetClientSocketFD();
				std::string full_buffer_client = Concerned_Buffers[socket];
				full_buffer_client.append(newbie_buffer);
				Concerned_Buffers[socket] = full_buffer_client;
				if (it->first->getMemberOfThisChan().size() > 0)
				{
						for (size_t i = 0; i < it->first->getMemberOfThisChan().size(); i++)
						{
							if (my_client->getNickname() != it->first->getMemberOfThisChan()[i])
							{
								std::string my_buffer = ":" + my_client->getMyServer()->GetServerName() + " 353 " + my_client->getNickname() + " = " + nameChan + " :" + all_names + "\r\n";
								my_buffer += ":" + my_client->getMyServer()->GetServerName() + " PRIVMSG " + nameChan + " :" + my_client->getNickname() + " has arrived\r\n";
								int socket = my_client->getMyServer()->find_destination(my_client->getMyServer()->send_back_channel(nameChan)->getMemberOfThisChan()[i])->GetClientSocketFD();
								std::string full_buffer_client = Concerned_Buffers[socket];
								full_buffer_client.append(my_buffer);
								Concerned_Buffers[socket] = full_buffer_client;
							}
						}
				}
				i++;
				it = chanList.begin();
			}
			
		}
		is_ready = true;
		server->addChannelList(chanList);
		is_not_accepted = false;
		setStatus(NOT_ALL_SEND);
	}
}



std::map<Channel *, bool> Command::parsingJoin(std::string command_leftovers, std::map<Channel *, bool> chanList)
{
	int count = 0;
	std::string firstchan;
	std::string secondChan;

	int j = 0;
	char buffer[20];
	size_t i = 0;

	std::vector<std::string> tabSeg;
	std::istringstream ss(command_leftovers);
	std::string seg;
	while (std::getline(ss, seg, ','))
	{
		tabSeg.push_back(seg);
		count += 1;
	}
	while (i < tabSeg.size() && j < count)
	{
		if (!chanList.empty())
		{
			std::map<Channel*, bool>::iterator it;
			for (it = chanList.begin(); it != chanList.end(); it++)
			{
				if (tabSeg[i] == it->first->getNameChannel() && it->first->getMemberOfThisChan().size() < MAX_MEMBERS_PER_CHANNEL) //si tu trouves la channel, juste ajouter le client dedans
				{
					it->first->addMember(my_client->getNickname());
					// std::cerr << "CHANNEL ADDED MEMBER TO " << tabSeg[i] << "" << std::endl;
					break ;
				}
				else if (tabSeg[i] == it->first->getNameChannel() && it->first->getMemberOfThisChan().size() == MAX_MEMBERS_PER_CHANNEL)
				{
					// std::cerr << "CHANNEL = " << tabSeg[i] << " is FULL" << std::endl;
					break ;
				}
			}
			if (it == chanList.end() && chanList.size() < MAX_CHANNELS) //sinon creation de la channel et ajout du client dedans
			{
				// std::cerr << "CHANNEL = " << tabSeg[i] << " has been CREATED" << std::endl;
				sprintf(buffer, "%d", j);
				std::string channelVersion = "Channel_";
				channelVersion.append(buffer);
				Channel *channelise = new Channel(channelVersion, tabSeg[i]);
				channelise->addMember(my_client->getNickname());
				chanList.insert(std::make_pair(channelise, true));
			}
			else if (chanList.size() == MAX_CHANNELS)
			{
				// std::cerr << "CHANNEL ARE ALL FULL" << std::endl;
				continue ;
			}
		}
		else if (chanList.size() < MAX_CHANNELS)
		{
			// std::cerr << "EMPTY MAP CHANNEL = " << tabSeg[i] << " has been CREATED" << std::endl;
			sprintf(buffer, "%d", j);
			std::string channelVersion = "Channel_";
			channelVersion.append(buffer);
			Channel *channelise = new Channel(channelVersion, tabSeg[i]);
			channelise->addMember(my_client->getNickname());
			chanList.insert(std::make_pair(channelise, true));
		}
		j++;
		i++;
	}
	return (chanList);
}
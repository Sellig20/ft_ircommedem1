#include "../includes/Command.hpp"
#include "../includes/Channel.hpp"
#include <string>
#include <algorithm>

std::vector<std::string> Command::eraseUserFromChan(std::vector<std::string> memberOfThisChan, std::string userNickname, Client *my_client)
{
	(void)my_client;
	if (!memberOfThisChan.empty())
	{
		for (std::vector<std::string>::iterator it = memberOfThisChan.begin(); it != memberOfThisChan.end(); it++)
		{
			if (memberOfThisChan.size() == 0)
				_flagShouldCloseChan = true;
			if (*it == userNickname)
			{
				memberOfThisChan.erase(it);
				break;
			}
		}
	}
	return memberOfThisChan;
}

std::string Command::builtReasonWhy(std::vector<std::string> toReason)
{
	std::string sample;
	size_t j = 0;

	while (j < toReason.size())
	{
		sample.append(toReason[j]);
		sample.append(" ");
		j++;
	}
	return (sample);
}

void Command::part()
{
	Server *server = my_client->getMyServer();

	std::map<Channel *, bool> &chanList = server->GetChannelList();
	std::vector<std::string> tabSeg;
	std::vector<std::string> tabSave;

	std::istringstream ss(command_leftovers);
	std::string seg;

	std::string reasonWhy;
	std::vector<std::string> toReason;
	
	if (command_leftovers.find(',') != std::string::npos)
	{
		//SI Y A PLSRS CHANNEL A FERMER
		std::string sample;
		while (std::getline(ss, seg, ','))
		{
			tabSave.push_back(seg);//il a CHANNELS TO CLOSE + REASON
		}
		sample = tabSave.back();
		tabSave.pop_back();
		std::istringstream ss1(sample);
		std::string seg1;
		int count = 0;
		while (std::getline(ss1, seg1, ' '))
		{
			count += 1;
			tabSeg.push_back(seg1);//il a REASON = last chan + reason
		}
		if (count > 1)
		{
			tabSave.push_back(tabSeg.front());
			for (size_t i = 1; i < tabSeg.size(); i++)
					toReason.push_back(tabSeg[i]);
			_reasonWhy = builtReasonWhy(toReason);
			_flagIsThereAReason = true;
		}
		else
			tabSave.push_back(sample);
		const long unsigned int found = tabSave[0].find(' ');
		if (found != std::string::npos)
			tabSave[0] = tabSave[0].substr(0, found);
		for (std::map<Channel *, bool>::iterator it = chanList.begin(); it != chanList.end(); it++)//boucle sur chaque channel
		{
			std::string channelHashtag = tabSave.front();
			std::string channelDataBase = it->first->getNameChannel();
			std::string connectedClientPart = my_client->getNickname();
			std::vector<std::string> vectorOfConnectedClientDB = it->first->getMemberOfThisChan();
			for (std::vector<std::string>::iterator ita = vectorOfConnectedClientDB.begin(); ita != vectorOfConnectedClientDB.end(); ita++)//boucle sur chaque client connecte
			{
				std::vector<std::string>::iterator itArgPart;
				for (itArgPart = tabSave.begin(); itArgPart != tabSave.end(); itArgPart++)//boucle sur channel recue du client 
				{
					if ((*itArgPart)[0] != '#' && channelDataBase[0] == '#')
						channelDataBase.erase(0, 1);
					else if ((*itArgPart)[0] == '#' && channelDataBase[0] != '#')
						channelDataBase.insert(0, "#");
					if (channelDataBase == *itArgPart && ita != vectorOfConnectedClientDB.end() && it->second == true)
					{
						response_buffer.append(":" + connectedClientPart + "!" + my_client->getUsername() + "@localhost PART " + channelDataBase + " " + *itArgPart);
						if (_flagIsThereAReason == true)
							response_buffer.append(" because : " + _reasonWhy + "\r\n");
						else
							response_buffer.append("\r\n");
						is_ready = true;
						is_not_accepted = false;
						setConcernedClients(connectedClientPart);
						status = SINGLE_SEND;
						it->first->setMemberOfThisChan(eraseUserFromChan(vectorOfConnectedClientDB, connectedClientPart, my_client));
						if (_flagShouldCloseChan == true)
							it->second = false;
						break ;
					}
					else if (channelDataBase == *itArgPart && ita == vectorOfConnectedClientDB.end() && it->second == true)
					{
						error_code = "442";
						response_buffer.append(connectedClientPart + " " + channelDataBase + " :You're not on that channel\n");
						is_ready = true;
						is_not_accepted = true;
						setConcernedClients(connectedClientPart);
						break;
					}
					// else
					// 	break;
				}		// std::cout << "BREEEAAAAAKKKK ELSEEEEEE" << std::endl;
			}
		}
	}
	else //SI Y A UNE CHANNEL A FERMER
	{
		while (std::getline(ss, seg, ' '))
		{
			tabSeg.push_back(seg);
			tabSave.push_back(seg);
		}
		if (tabSeg.size() == 1)
			_flagIsThereAReason = false;
		else if (tabSeg.size() > 1)
		{
			tabSeg[0].erase(0,1);
			tabSeg[1].erase(0,1);
			if (tabSeg[0] == tabSeg[1])
			{
				if (tabSeg.size() > 2)
				{
					tabSeg.erase(tabSeg.begin());
					tabSeg.erase(tabSeg.begin());
					for (size_t i = 0; i < tabSeg.size(); i++)
						toReason.push_back(tabSeg[i]);
					_reasonWhy = builtReasonWhy(toReason);
					_flagIsThereAReason = true;
				}
				else
					_flagIsThereAReason = false;
			}
			else
			{
				tabSeg.erase(tabSeg.begin());
				tabSeg[0].erase(0, 1);
				for (size_t i = 0; i < tabSeg.size(); i++)
					toReason.push_back(tabSeg[i]);
				_reasonWhy = builtReasonWhy(toReason);
				_flagIsThereAReason = true;
			}
		}
		for (std::map<Channel *, bool>::iterator it = chanList.begin(); it != chanList.end(); it++)
		{
			std::string channelHashtag = tabSave.front();
			std::string channelDataBase = it->first->getNameChannel();
			std::string connectedClientPart = my_client->getNickname();
			std::vector<std::string> vectorOfConnectedClientDB = it->first->getMemberOfThisChan();
			for (std::vector<std::string>::iterator ita = vectorOfConnectedClientDB.begin(); ita != vectorOfConnectedClientDB.end(); ita++) //NOM DU CLIENT CONNECTE A LA CHANNEL A QUITTER
			{
				std::vector<std::string>::iterator itArgPart;
				for (itArgPart = tabSave.begin(); itArgPart != tabSave.end(); itArgPart++)//boucle sur channel recue du client 
				{
					if ((*itArgPart)[0] != '#' && channelDataBase[0] == '#')
						channelDataBase.erase(0, 1);
					else if ((*itArgPart)[0] == '#' && channelDataBase[0] != '#')
						channelDataBase.insert(0, "#");
					if (channelDataBase == *itArgPart && ita != vectorOfConnectedClientDB.end() && it->second == true)
					{
						response_buffer.append(":" + connectedClientPart + "!" + my_client->getUsername() + "@localhost PART " + channelDataBase + " " + *itArgPart);
						if (_flagIsThereAReason == true)
							response_buffer.append(" because : " + _reasonWhy + "\r\n");
						else
							response_buffer.append("\r\n");
						is_ready = true;
						is_not_accepted = false;
						setConcernedClients(connectedClientPart);
						status = SINGLE_SEND;
						it->first->setMemberOfThisChan(eraseUserFromChan(vectorOfConnectedClientDB, connectedClientPart, my_client));
						if (_flagShouldCloseChan == true)
							it->second = false;
						return ;
					}
					else if (channelDataBase == *itArgPart && ita == vectorOfConnectedClientDB.end() && it->second == true)
					{
						error_code = "442";
						response_buffer.append(connectedClientPart + " " + channelDataBase + " :You're not on that channel\n");
						is_ready = true;
						is_not_accepted = true;
						setConcernedClients(connectedClientPart);
						return ;
					}
					else
						break;
				}
				if (itArgPart == tabSave.end())
				{
					error_code = "403";
					response_buffer.append(connectedClientPart + " " + channelDataBase + " :No such channel\n");
					is_ready = true;
					is_not_accepted = true;
					setConcernedClients(connectedClientPart);
					return ;
				}
			}
		}		
	}
}
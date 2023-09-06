#include "../includes/Command.hpp"
#include "../includes/Channel.hpp"
#include <string>
#include <algorithm>

std::vector<std::string> Command::eraseUserFromChan(std::vector<std::string> memberOfThisChan, std::string userNickname, Client *my_client)
{
	(void)my_client;
	// std::cout << "to erase member " << userNickname << std::endl;
	if (!memberOfThisChan.empty())
	{
		for (std::vector<std::string>::iterator it = memberOfThisChan.begin(); it != memberOfThisChan.end(); it++)
		{
			std::cout << ">>>>>>>>>>>>>>>>>>>> " << *it << " ||||||||||||||||||||||| " << memberOfThisChan.size() << std::endl;
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
	std::cout << " ????????????????????? JE SUIS " << my_client->getNickname() << " ????????????????????????? " << std::endl;

	std::map<Channel *, bool> &chanList = server->GetChannelList();
	std::vector<std::string> tabSeg;
	std::vector<std::string> tabSave;

	//ON VA RECUP COMMAND_LEFTOVERS POUR PARSER CHANNEL VS REASON
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
			std::cout << ">>>>>> seg" << seg1 << std::endl;
		}
		if (count > 1)
		{
			std::cout << ">>>>>> tab seg front " << tabSeg.front() << std::endl;
			tabSave.push_back(tabSeg.front());
			for (size_t i = 1; i < tabSeg.size(); i++)
					toReason.push_back(tabSeg[i]);
			_reasonWhy = builtReasonWhy(toReason);
			_flagIsThereAReason = true;
		}
		else
			tabSave.push_back(sample);
		std::cout << std::endl;
		std::cout << "????????? " << _reasonWhy << std::endl;
		std::cout << "??????? " << tabSave << std::endl;
		std::cout << std::endl;
		const long unsigned int found = tabSave[0].find(' ');
		if (found != std::string::npos)
		{
			tabSave[0] = tabSave[0].substr(0, found);
		}
			std::cout << "::::::::::::::::::::::: " << tabSave << std::endl;
		for (std::map<Channel *, bool>::iterator it = chanList.begin(); it != chanList.end(); it++)
		{
			std::string channelHashtag = tabSave.front(); // NOM DE LA CHANNEL DEPUIS ARGV
			// std::cout << "nom de channel enregistrees dans mon serveur : " << it->first->getNameChannel() << std::endl;
			std::string channelDataBase = it->first->getNameChannel(); //NOM DE LA CHANNEL DEPUIS MA BASE DE DONNEE (celles qui sont donc passees par join)
			std::string connectedClientPart = my_client->getNickname(); //NOM DE MON CLIENT QUI A TAPE /PART
			std::vector<std::string> vectorOfConnectedClientDB = it->first->getMemberOfThisChan(); //VECTOR DES CLIENTS CONNECTES A LA CHANNEL A QUITTER
			if (it->second == true)
				std::cout << " ??????????????????????????   " << it->first->getMemberOfThisChan() << " ????????????????????? name channel = " << it->first->getNameChannel() << std::endl;

			//je loop dans les arguments envoyes a /PART pour faire correspondre la channel a quitter avec les channels enregistrees dans ma DB et le client qui veut quitter avec les clients enregistres dans cette channel quil veut quitter...
			// for (std::vector<std::string>::iterator itArgPart = tabSave.begin(); itArgPart != tabSave.end(); itArgPart++)
			for (std::vector<std::string>::iterator ita = vectorOfConnectedClientDB.begin(); ita != vectorOfConnectedClientDB.end(); ita++) //NOM DU CLIENT CONNECTE A LA CHANNEL A QUITTER
			{
				std::vector<std::string>::iterator itArgPart;
				for (itArgPart = tabSave.begin(); itArgPart != tabSave.end(); itArgPart++)
				{
					std::cout << "channel database = " << channelDataBase << " | *itArgPart = " << *itArgPart << std::endl;
					if (ita != vectorOfConnectedClientDB.end())
						std::cout << "ita = " << *ita << std::endl;
					else
					{
						std::cout << "pas de ITA donc pas trouve " << connectedClientPart << std::endl;
						for (std::vector<std::string>::iterator itu = vectorOfConnectedClientDB.begin(); itu != vectorOfConnectedClientDB.end(); itu++)
						{
							std::cout << "Connected client == " << *itu << std::endl;
						}
					}
					if (channelDataBase == *itArgPart && ita != vectorOfConnectedClientDB.end() && it->second == true)
					{
						std::cout << "je vais erased !" << std::endl;
						std::string channelWoHash;
						if (!channelDataBase.empty())
							channelWoHash = channelDataBase.erase(0, 1);
						response_buffer.append(":" + connectedClientPart + "!" + my_client->getUsername() + "@localhost PART " + channelDataBase + " " + channelWoHash);
						if (_flagIsThereAReason == true)
							response_buffer.append(" because : " + _reasonWhy + "\r\n");
						else
						{
							std::cout << "SPACE ELSE PART" << std::endl;
							response_buffer.append("\r\n");
						}
						is_ready = true;
						is_not_accepted = false;
						setConcernedClients(connectedClientPart);
						status = SINGLE_SEND;
						std::cout << ">>>>>>>>>>>>>>>>>>>> for " << my_client->getNickname() << " | " << it->first->getNameChannel() << std::endl; 
						it->first->setMemberOfThisChan(eraseUserFromChan(vectorOfConnectedClientDB, connectedClientPart, my_client));
						if (_flagShouldCloseChan == true)
						{
							std::cout << "chanlist erased !!!!!!!!" << std::endl;
							it->second = false;
						}
					}
					else if (channelDataBase == *itArgPart && ita == vectorOfConnectedClientDB.end() && it->second == true)
					{
						error_code = "442";
						response_buffer.append(connectedClientPart + " " + channelDataBase + " :You're not on that channel\n");
						is_ready = true;
						is_not_accepted = true;
						setConcernedClients(connectedClientPart);
					}
					else
						std::cout << "BREEEAAAAAKKKK ELSEEEEEE" << std::endl;
						// break;
				}
				if (itArgPart == tabSave.end())
				{
					std::cout << "DANS NO SUUUUUUUUUCH CHANNEL" << std::endl;
					error_code = "403";
					response_buffer.append(connectedClientPart + " " + channelDataBase + " :No such channel\n");
					is_ready = true;
					is_not_accepted = true;
					setConcernedClients(connectedClientPart);
				}
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
		{
			// std::cout << "-----1------------ #mia ----------------------------" << std::endl;
			// std::cout << "Reponse 1 = " << command_leftovers << std::endl;
			_flagIsThereAReason = false;
		}
		else if (tabSeg.size() > 1)
		{
			tabSeg[0].erase(0,1);
			tabSeg[1].erase(0,1);
			if (tabSeg[0] == tabSeg[1])
			{
				if (tabSeg.size() > 2)
				{
					// std::cout << "-----------2--------- #mia :mia jaime pas ------------------------" << std::endl;
					// std::cout << "Reponse 2 = " << command_leftovers << std::endl;
					tabSeg.erase(tabSeg.begin());
					tabSeg.erase(tabSeg.begin());
					for (size_t i = 0; i < tabSeg.size(); i++)
						toReason.push_back(tabSeg[i]);
					_reasonWhy = builtReasonWhy(toReason);
					_flagIsThereAReason = true;
				}
				else
				{
					// std::cout << "----------3------- #mia :mia -----------------------" << std::endl;
					// std::cout << "Reponse 3 = " << command_leftovers << std::endl;
					_flagIsThereAReason = false;
				}
			}
			else
			{
				// std::cout << "-------4-------- #mia :j'aime pas -------------------------" << std::endl;
				// std::cout << "Reponse 4 = " << command_leftovers << std::endl;
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
			std::string channelHashtag = tabSave.front(); // NOM DE LA CHANNEL DEPUIS ARGV
			// std::cout << "nom de channel enregistrees dans mon serveur : " << it->first->getNameChannel() << std::endl;
			std::string channelDataBase = it->first->getNameChannel(); //NOM DE LA CHANNEL DEPUIS MA BASE DE DONNEE (celles qui sont donc passees par join)
			std::string connectedClientPart = my_client->getNickname(); //NOM DE MON CLIENT QUI A TAPE /PART
			std::vector<std::string> vectorOfConnectedClientDB = it->first->getMemberOfThisChan(); //VECTOR DES CLIENTS CONNECTES A LA CHANNEL A QUITTER
			if (it->second == true)
				std::cout << " ??????????????????????????   " << it->first->getMemberOfThisChan() << " ????????????????????? name channel = " << it->first->getNameChannel() << std::endl;

			//je loop dans les arguments envoyes a /PART pour faire correspondre la channel a quitter avec les channels enregistrees dans ma DB et le client qui veut quitter avec les clients enregistres dans cette channel quil veut quitter...
			// for (std::vector<std::string>::iterator itArgPart = tabSave.begin(); itArgPart != tabSave.end(); itArgPart++)
			for (std::vector<std::string>::iterator ita = vectorOfConnectedClientDB.begin(); ita != vectorOfConnectedClientDB.end(); ita++) //NOM DU CLIENT CONNECTE A LA CHANNEL A QUITTER
			{
				std::vector<std::string>::iterator itArgPart = tabSave.begin();
				while (itArgPart != tabSave.end())
				{
					std::cout << "channel database = " << channelDataBase << " | *itArgPart = " << *itArgPart << std::endl;
					if (ita != vectorOfConnectedClientDB.end())
						std::cout << "ita = " << *ita << std::endl;
					else
					{
						std::cout << "pas de ITA donc pas trouve " << connectedClientPart << std::endl;
						for (std::vector<std::string>::iterator itu = vectorOfConnectedClientDB.begin(); itu != vectorOfConnectedClientDB.end(); itu++)
						{
							std::cout << "Connected client == " << *itu << std::endl;
						}
					}
					if (channelDataBase == *itArgPart && ita != vectorOfConnectedClientDB.end() && it->second == true)
					{
						std::cout << "je vais erased !" << std::endl;
						std::string channelWoHash;
						if (!channelDataBase.empty())
							channelWoHash = channelDataBase.erase(0, 1);
						response_buffer.append(":" + connectedClientPart + "!" + my_client->getUsername() + "@localhost PART " + channelDataBase + " " + channelWoHash);
						if (_flagIsThereAReason == true)
							response_buffer.append(" because : " + _reasonWhy + "\r\n");
						else
						{
							std::cout << "SPACE ELSE PART" << std::endl;
							response_buffer.append("\r\n");
						}
						is_ready = true;
						is_not_accepted = false;
						setConcernedClients(connectedClientPart);
						status = SINGLE_SEND;
						std::cout << ">>>>>>>>>>>>>>>>>>>> for " << my_client->getNickname() << " | " << it->first->getNameChannel() << std::endl; 
						it->first->setMemberOfThisChan(eraseUserFromChan(vectorOfConnectedClientDB, connectedClientPart, my_client));
						if (_flagShouldCloseChan == true)
						{
							std::cout << "chanlist erased !!!!!!!!" << std::endl;
							it->second = false;
						}
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
					std::cout << "DANS NO SUUUUUUUUUCH CHANNEL" << std::endl;
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
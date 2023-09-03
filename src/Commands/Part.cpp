#include "../includes/Command.hpp"
#include "../includes/Channel.hpp"
#include <string>
#include <algorithm>

std::vector<std::string> Command::eraseUserFromChan(std::vector<std::string> memberOfThisChan, std::string userNickname)
{
	std::cout << "to erase member " << userNickname << std::endl;
	if (!memberOfThisChan.empty())
	{
		for (std::vector<std::string>::iterator it = memberOfThisChan.begin(); it != memberOfThisChan.end(); it++)
		{
			if (*it == userNickname)
			{
				memberOfThisChan.erase(it);
			}
			if (memberOfThisChan.size() == 0)
			{
				_flagShouldCloseChan = true;
				break ;
			}
			else if (memberOfThisChan.size() > 0)
				std::cout << "il reste : " << *it << std::endl;
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

	std::map<Channel *, bool> chanList = server->GetChannelList();
	// std::vector<std::string> memberOfThisChan;
	std::vector<std::string> tabSeg;
	std::vector<std::string> tabSave;

	// std::string channelPoint;

	std::cout << "ARGV => " << command_leftovers << std::endl;

	//ON VA RECUP COMMAND_LEFTOVERS POUR PARSER CHANNEL VS REASON
	std::istringstream ss(command_leftovers);
	std::string seg;

	std::string r_easonWhy;
	std::vector<std::string> toReason;
	
	if (command_leftovers.find(',') != std::string::npos)
	{
		//SI Y A PLSRS CHANNEL A FERMER
		exit(1);
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
			std::cout << "-----1------------ #mia ----------------------------" << std::endl;
			std::cout << "Reponse 1 = " << command_leftovers << std::endl;
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
					std::cout << "-----------2--------- #mia :mia jaime pas ------------------------" << std::endl;
					std::cout << "Reponse 2 = " << command_leftovers << std::endl;
					tabSeg.erase(tabSeg.begin());
					tabSeg.erase(tabSeg.begin());
					for (size_t i = 0; i < tabSeg.size(); i++)
						toReason.push_back(tabSeg[i]);
					_reasonWhy = builtReasonWhy(toReason);
					_flagIsThereAReason = true;
				}
				else
				{
					std::cout << "----------3------- #mia :mia -----------------------" << std::endl;
					std::cout << "Reponse 3 = " << command_leftovers << std::endl;
					_flagIsThereAReason = false;
				}
			}
			else
			{
				std::cout << "-------4-------- #mia :j'aime pas -------------------------" << std::endl;
				std::cout << "Reponse 4 = " << command_leftovers << std::endl;
				tabSeg.erase(tabSeg.begin());
				tabSeg[0].erase(0, 1);
				for (size_t i = 0; i < tabSeg.size(); i++)
					toReason.push_back(tabSeg[i]);
				_reasonWhy = builtReasonWhy(toReason);
				_flagIsThereAReason = true;
			}
		}
	}
	std::string channelHashtag = tabSave.front(); // NOM DE LA CHANNEL DEPUIS ARGV
	for (std::map<Channel *, bool>::iterator it = chanList.begin(); it != chanList.end(); it++)
	{
		std::cout << "nom de channel enregistrees dans mon serveur : " << it->first->getNameChannel() << std::endl;
		std::string channelDataBase = it->first->getNameChannel(); //NOM DE LA CHANNEL DEPUIS MA BASE DE DONNEE (celles qui sont donc passees par join)
		std::string connectedClientPart = my_client->getNickname(); //NOM DE MON CLIENT QUI A TAPE /PART
		std::vector<std::string> vectorOfConnectedClientDB = it->first->getMemberOfThisChan(); //VECTOR DES CLIENTS CONNECTES A LA CHANNEL A QUITTER
		std::vector<std::string>::iterator ita = std::find(vectorOfConnectedClientDB.begin(), vectorOfConnectedClientDB.end(), connectedClientPart); //NOM DU CLIENT CONNECTE A LA CHANNEL A QUITTER

		//je loop dans les arguments envoyes a /PART pour faire correspondre la channel a quitter avec les channels enregistrees dans ma DB et le client qui veut quitter avec les clients enregistres dans cette channel quil veut quitter...
		for (std::vector<std::string>::iterator itArgPart = tabSave.begin(); itArgPart != tabSave.end(); itArgPart++)
		{
			std::cout << "channel database = " << channelDataBase << " | *itArgPart = " << *itArgPart << std::endl;
			std::cout << "ita = " << *ita << std::endl;
			if (channelDataBase == *itArgPart && ita != vectorOfConnectedClientDB.end() && it->second == true)
			{
				std::cout << "Channel founded" << std::endl;
				std::string channelWoHash;
				if (!channelDataBase.empty())
					channelWoHash = channelDataBase.erase(0, 1);
				response_buffer.append(":" + connectedClientPart + "!" + my_client->getUsername() + "@localhost PART " + channelDataBase + " " + channelWoHash);
				if (_flagIsThereAReason == true)
					response_buffer.append(" because : " + _reasonWhy + "\r\n");
				else
					response_buffer.append("\r\n");
				is_ready = true;
				is_not_accepted = false;
				setConcernedClients(connectedClientPart);
				status = SINGLE_SEND;
				vectorOfConnectedClientDB = eraseUserFromChan(vectorOfConnectedClientDB, connectedClientPart);
				if (_flagShouldCloseChan == true)
				{
					std::cout << "chanlist erase !!!!!!!!" << std::endl;
					it->second = false;
				}
				displayChannelAndMember(chanList);
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
 	// 		if (channelToFind == *itTS && ita != memberOfThisChan.end() && it->second == true)
	// 		{
	// 			std::string channelToFind = it->first->getNameChannel();
			
	// 			if (_flagPart == 1)
	// 				response_buffer.append(" because : " + _reasonWhy);

	// 			memberOfThisChan = eraseUserFromChan(memberOfThisChan, clientNickname);

	// 			std::cout << "Pour cette chan : " << channelToFind << std::endl;
	// 			std::cout << "after erase, mon flag isopen => " << it->second << std::endl;

	// 			if (_flagIsCloseChan == true)
	// 			{
	// 				it->second = false;
	// 				std::cout << "after FLAGISCLOSECHAN, mon flag isopen => " << it->second << std::endl;
	// 				std::cout << "je dois close chan mais j'y arrive pas" << std::endl;
	// 			}
	// 			// break;
	// 		}
	// 		else if (channelToFind == *itTS && *ita != clientNickname && it->second == true)
	// 		{
	// 			error_code = "442";
	// 			response_buffer.append(clientNickname + " " + channelToFind + " :You're not on that channel\n");
	// 			is_ready = true;
	// 			is_not_accepted = true;
	// 			setConcernedClients(clientNickname);
	// 		}
	// 		else
	// 		{
	// 			error_code = "403";
	// 			response_buffer.append(clientNickname + " " + channelToFind + " :No such channel\n");
	// 			is_ready = true;
	// 			is_not_accepted = true;
	// 			setConcernedClients(clientNickname);
	// 		}
	// 	}
	// }
	// std::cout << std::endl;
	// std::cout << std::endl;
	// std::cout << "After part :" << std::endl;
	// displayChannelAndMember();
	// std::cout << std::endl;
	// std::cout << std::endl;







// 			
// 			{// = client n'as PAS mis le #
// 				channelHashtag = tabSeg[0];
// 				channelPoint = tabSeg[1];
// 				
// 				_flagSpace = 2;
// 			}
// 			else //client a MIS le #
// 			{
// 				channelHashtag = tabSeg[0];
// 				
// 				_flagSpace = 1;
// 			}
// 			
// 		}
// 		else // SI 1 CHANNEL 0 REASON
	// 	{
	// 		std::cout << "---------------------------------- 1 channel to close, 0 reason -----------------------------------------" << std::endl;
	// 		channelHashtag = tabSeg[0];
	// 	}
	// }
	// if (toReason.size() > 1)
	// {
	// 	
	// 	_flagPart = 1;
	// }
	// else
	// 	_flagPart = 2;

	// 
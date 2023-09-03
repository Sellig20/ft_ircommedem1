#include "../includes/Command.hpp"
#include "../includes/Channel.hpp"
#include <string>
#include <algorithm>

std::vector<std::string> Command::eraseUserFromChan(std::vector<std::string> memberOfThisChan, std::string userNickname)
{
	// size_t i = 0;
	if (!memberOfThisChan.empty())
	{
		for (std::vector<std::string>::iterator it = memberOfThisChan.begin(); it != memberOfThisChan.end(); it++)
		{
			if (*it == userNickname)
			{
				std::cout << " >>>>>>>>>>>>> it : " << *it << std::endl;
				std::cout << " name dans erase : " << userNickname;
				memberOfThisChan.erase(it);
				std::cout << "a priori j'ai erased member" << std::endl;
			}
			std::cout << "Taille de memeberOFthisChan = " << memberOfThisChan.size() << std::endl;
			if (memberOfThisChan.size() == 0)
			{
				_flagIsCloseChan = true;
				break;
				// memberOfThisChan.push_back("AABBCCDDEEFFGGHHIIJJKKLLMMNNOOPP");
				// std::cout << "jai push back ABCDEFGHJIKLMNOP" << std::endl;
			}
			if (memberOfThisChan.size() > 0)
				std::cout << "il reste : " << *it << std::endl;
		}
	}
	std::cout << "Avant erase return" << std::endl;
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

	// std::string channelHashtag;
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
			// std::cout << ">> " << seg << std::endl;
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
				if (!tabSeg[2].empty())
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
	exit(1);	
}


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

	// for (std::map<Channel *, bool>::iterator it = chanList.begin(); it != chanList.end(); it++)
	// {//LOOP DANS MES CHANNELS

	// 	std::string channelToFind = it->first->getNameChannel(); //PROBLEME
	// 	//je prends le nom de la channel enregistree

	// 	// std::string channelToFind = channelHashtag;
	// 	std::string clientNickname = my_client->getNickname();
	// 	//je prends le nom du client enregistre connecte a cettte channel

	// 	memberOfThisChan = it->first->getMemberOfThisChan(); //PROBLEME
	// 	//je mets les clients connectes a cette channel dans un vecteur memberOfThisChan

	// 	std::vector<std::string>::iterator itTS;
	// 	for (itTS = tabSave.begin(); itTS != tabSave.end(); itTS++)
	// 	{//je vais chercher les noms de channel de mon argv
	// 		std::vector<std::string>::iterator ita = std::find(memberOfThisChan.begin(), memberOfThisChan.end(), clientNickname);
	// 		//je vais chercher le nom de client connecte

	// 		std::cout << "Channeltofind = " << channelToFind << " | itTS = " << *itTS << std::endl;
	// 		std::cout << "ita = " << *ita << std::endl;
 	// 		if (channelToFind == *itTS && ita != memberOfThisChan.end() && it->second == true)
	// 		{
	// 			std::string channelWoHash;
	// 			if (!channelToFind.empty())
	// 				channelWoHash = channelToFind.erase(0, 1);
	// 			std::string channelToFind = it->first->getNameChannel();
			
	// 			response_buffer.append(":" + clientNickname + "!" + my_client->getUsername() + "@localhost PART " + channelToFind + " " + channelWoHash + "\r\n");
	// 			if (_flagPart == 1)
	// 				response_buffer.append(" because : " + _reasonWhy);
	// 			is_ready = true;
	// 			is_not_accepted = false;
	// 			setConcernedClients(clientNickname);
	// 			status = SINGLE_SEND;
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
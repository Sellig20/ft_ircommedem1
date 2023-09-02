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
				memberOfThisChan.erase(it);
				break;
			}
			std::cout << ">>>>>>>>>>>>> " << *it << std::endl;
		}
	}
	return memberOfThisChan;
}

std::string Command::builtReasonWhy(std::vector<std::string> reason)
{
	std::string sample;
	for(size_t j = 0; j < reason.size(); j ++)
	{
		sample.append(reason[j]);
		sample.append(" ");
	}
	return (sample);
}

void Command::part()
{
	Server *server = my_client->getMyServer();

	std::map<Channel *, bool> chanList = server->GetChannelList();
	std::vector<std::string> memberOfThisChan;
	std::vector<std::string> tabSeg;

	std::istringstream ss(command_leftovers);
	std::string seg;

	std::string ind;
	while (std::getline(ss, seg, ','))
	{
		tabSeg.push_back(seg);
	}
	ind = tabSeg.back();

	std::istringstream ss1(ind);
	std::string r;
	std::vector<std::string> reason;
	while (std::getline(ss1, r, ' '))
	{
		reason.push_back(r);
	}
	tabSeg.pop_back();
	tabSeg.push_back(reason.front());
	reason.erase(reason.begin());
	for(size_t j = 0; j < tabSeg.size(); j++)
	{
		std::cout << "Channel to quit -----------> " << tabSeg[j] << std::endl;
	}
	
	std::string reasonWhy = builtReasonWhy(reason);
	// std::cout << "Reason why -----------> " << reasonWhy << std::endl;
	for (std::map<Channel *, bool>::iterator it = chanList.begin(); it != chanList.end(); it++)
	{//LOOP DANS MES CHANNELS 
		std::string channelToFind = it->first->getNameChannel();
		// std::string clientName = my_client->getname();
		std::string clientNickname = my_client->getNickname();
		std::cout << "clientName = " << clientNickname ;


		memberOfThisChan = it->first->getMemberOfThisChan();
		std::vector<std::string>::iterator itTS;

		for (itTS = tabSeg.begin(); itTS != tabSeg.end(); itTS++)
		{//LOOP DANS LE NOM DE CHANNEL ENVOYE EN ARGV
			std::vector<std::string>::iterator ita = std::find(memberOfThisChan.begin(), memberOfThisChan.end(), clientNickname);
			//FIND LOOP DANS LES MEMBRES ACTIFS DE LA CHANNEL ACTIVE
			std::cout << "channelToFind = " << channelToFind << " | *itTs = " << *itTS <<std::endl;
			std::cout << " | *ita = " << *ita << std::endl;
			if (channelToFind == *itTS && ita != memberOfThisChan.end() && it->second == true)
			{
				std::cout << "OK channel trouvee !" << std::endl;

				error_code = "!";
				response_buffer.append(" is leaving the channel");
				is_ready = true;
				setConcernedClients(clientNickname);
				memberOfThisChan = eraseUserFromChan(memberOfThisChan, clientNickname);
				break;
			}
			else if (channelToFind == *itTS && *ita != clientNickname && it->second == true)
			{
				error_code = "442";
				response_buffer.append(" You're not on that channel");
				is_ready = true;
				setConcernedClients(clientNickname);
			}
			else
			{
				error_code = "403";
				response_buffer.append(" No such channel\n");
				is_ready = true;
				setConcernedClients(clientNickname);
			}
		}
		for (size_t y = 0; y < memberOfThisChan.size(); y++)
		{
			std::cout << "member apres erase : " << std::endl;
		}
	}
}
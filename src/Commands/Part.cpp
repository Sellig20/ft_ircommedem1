#include "../includes/Command.hpp"
#include "../includes/Channel.hpp"

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
	// std::cout << "++++++++++++ je suis dans part voici le leftorvers : " << command_leftovers << " +++++++++++++" << std::endl;
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
	{
		// std::cout << "!!! /PART ==> Actuellement dans " << it->first->getNumChannel() << " | nom = " << it->first->getNameChannel() << " par " << my_client->getUsername() << std::endl;
		// std::cout << "Member of this channel : " << it->first->getMemberOfThisChan() << std::endl;

		std::string channelToFind = it->first->getNameChannel();
		std::string clientName = my_client->getUsername();

		memberOfThisChan = it->first->getMemberOfThisChan();
		std::vector<std::string>::iterator itTS;

		for(itTS = tabSeg.begin(); itTS != tabSeg.end(); itTS++)
		{
			for(std::vector<std::string>::iterator ita = memberOfThisChan.begin(); ita != memberOfThisChan.end(); ita++)
			{
				if (channelToFind == *itTS && *ita == clientName)
				{
					// std::cout << "channelToFind = " << channelToFind << " | *itTs = " << *itTS <<std::endl;
					// std::cout << "clientName = " << clientName << " | *ita = " << *ita << std::endl;
					std::cout << "OK channel trouvee !" << std::endl;
					response_buffer.append(":" + clientName + "!" + clientName + "@localhost PART " + channelToFind + " ;" + clientName + " is leaving the channel " + channelToFind);
					// memberOfThisChan.erase(ita);
				}
				else if (channelToFind == *itTS && *ita != clientName)
					std::cout << "you dont belong to the chan" << std::endl;
				else
					response_buffer.append(ERR_NOSUCHCHANNEL + clientName + " " + channelToFind + " :No such channel\n");
			}
		}
	}
}
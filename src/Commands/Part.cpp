#include "../includes/Command.hpp"
#include "../includes/Channel.hpp"

void Command::part()
{
	// std::cout << "++++++++++++ je suis dans part voici le leftorvers : " << command_leftovers << " +++++++++++++" << std::endl;
	Server *server = my_client->getMyServer();

	std::map<Channel *, bool> chanList = server->GetChannelList();
	std::vector<std::string> memberOfThisChan;
	std::vector<std::string> tabSeg;

	std::istringstream ss(command_leftovers);
	std::string seg;

	size_t ind = 0;
	while (std::getline(ss, seg, ','))
	{
		// tabSeg.push_back(seg);
		while (seg[ind])
		{
			if (seg[ind] == ' ')
				std::cout << "space : " << seg[ind] << std::endl;
			else
				std::cout << seg[ind] << std::endl;
			ind++;
		}
		
	}
	exit(1);
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
					std::cout << "OK channel trouvee !" << std::endl;
					// memberOfThisChan.erase(ita);
				}
				else if (channelToFind == *itTS && *ita != clientName)
					std::cout << "you dont belong to the chan" << std::endl;
				else
					std::cout << "Channel invalid bro" << std::endl;
			}
		}
	}
}
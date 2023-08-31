#include "../includes/Command.hpp"
#include "../includes/Channel.hpp"

void Command::join()
{
	std::string backToBuf;
	// std::cout << "++++++++++++ je suis dans join voici le leftovers : " << command_leftovers << "+++++++++++++" << std::endl;
    if (!command_leftovers.empty())
	{
		Server *server = my_client->getMyServer();
		std::map<Channel *, bool> chanList = server->GetChannelList();

		chanList = parsingJoin(command_leftovers, chanList);

		for (std::map<Channel *, bool>::iterator it = chanList.begin(); it != chanList.end(); it++)
		{
			// std::cout << "!!! /JOIN ==> Creation de " << it->first->getNumChannel() << " | nom = " << it->first->getNameChannel() << " par " << my_client->getUsername() << std::endl;
			// std::cout << "Member of this channel : " << it->first->getMemberOfThisChan() << std::endl;
		}
		server->addChannelList(chanList);
		// backToBuf.append(":" + my_client->getNickname() + "!" + my_client->getUsername() + "@localhost JOIN " + chan->getNameChannel() + "\n");
		// backToBuf.append(":" + my_client.getNickname() + "!" + my_client.getNickname() + "localhost 332 " + my_client.getNickname() + " " + chan->getNameChannel() + " :" + channel.getTopicChannel() + "\n"); 
	}
}

std::map<Channel *, bool> Command::parsingJoin(std::string command_leftovers, std::map<Channel *, bool> chanList)
{
	std::cout << "parsing join" << std::endl;
	int count = 0;
	std::string firstchan;
	std::string secondChan;
	std::vector<std::string> tabSeg;
	std::istringstream ss(command_leftovers);
	std::string seg;
	int j = 0;
	char buffer[20];
	size_t i = 0;

	while (std::getline(ss, seg, ','))
	{
		tabSeg.push_back(seg);
		count += 1;
	}
	while (i < tabSeg.size() && j < count)
	{
		sprintf(buffer, "%d", j);
		std::string channelVersion = "Channel_";
		channelVersion.append(buffer);

		Channel *channelise = new Channel(channelVersion, tabSeg[i]);
		
		channelise->addMember(my_client->getUsername());

		chanList.insert(std::make_pair(channelise, true));
		j++;
		i++;
	}
	// std::cout << "dans parsing de join" << std::endl;
	// for (std::map<Channel *, bool>::iterator it = chanList.begin(); it != chanList.end(); it++)
	// {
	// 	std::cout << "!!! /JOIN ==> Creation de " << it->first->getNumChannel() << " | nom = " << it->first->getNameChannel() << std::endl;
	// }
	return (chanList);
}
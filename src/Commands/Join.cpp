#include "../includes/Command.hpp"
#include "../includes/Channel.hpp"

void Command::join()
{
	std::cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
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
		for (std::map<Channel*, bool>::iterator it = chanList.begin(); it != chanList.end(); it++)
		{
			std::cout << "LES CHANNELS OUVERTES : " << std::endl;
			if (it->second == true)
			{
				std::cout << it->first->getNameChannel() << std::endl;
			}
			std::cout << "------------------------" << std::endl;
			std::cout << "LES CHANNELS FERMEES : " << std::endl;
			if (it->second == false)
			{
				std::cout << it->first->getNameChannel() << std::endl;
			}
			std::vector<std::string> memberOfThisChan;
			memberOfThisChan = it->first->getMemberOfThisChan();
			std::cout << "---------------" << std::endl;
			for (std::vector<std::string>::iterator ita = memberOfThisChan.begin(); ita != memberOfThisChan.end(); ita++)
			{
				std::cout << "Member of channel [" << it->first->getNameChannel() << "] : " << *ita << std::endl;
			}
		}
	}
}

#include <algorithm>
#include <string>

std::map<Channel *, bool> Command::parsingJoin(std::string command_leftovers, std::map<Channel *, bool> chanList)
{
	std::cout << std::endl;
	std::cout << "JE SUIS DANS PARSING JOIN" << std::endl;
	std::cout << std::endl;
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
		// std::cout << ">>>>>>>>>>>>>>>>> " << seg << std::endl;
	}
	while (i < tabSeg.size() && j < count)
	{
		if (!chanList.empty())
		{
			for (std::map<Channel*, bool>::iterator it = chanList.begin(); it != chanList.end(); it++)
			{
				if (it->first->getNameChannel() == tabSeg[i]) //si tu trouves la channel, juste ajouter le client dedans
					it->first->addMember(my_client->getNickname());
				else //sinon creation de la channel et ajout du client dedans
				{
					//NUMERO DE SERIE
					sprintf(buffer, "%d", j);
					std::string channelVersion = "Channel_";
					channelVersion.append(buffer);

					//CREATION
					Channel *channelise = new Channel(channelVersion, tabSeg[i]);

					//AJOUT DE MON CLIENT AUX MEMBRES ACTIFS DE LA CHANNEL ACTIVE QU'IL VIENT DE CREER
					channelise->addMember(my_client->getNickname());

					//INSERT DE NV CHAN + NV CLIENT A CHANLIST (DA BIG ONE)
					chanList.insert(std::make_pair(channelise, true));
				}
			}
		}
		else
		{
			// std::cout << "CHANLIST IS EMPTY" << std::endl;
			//NUMERO DE SERIE
			sprintf(buffer, "%d", j);
			std::string channelVersion = "Channel_";
			channelVersion.append(buffer);
			//CREATION
			Channel *channelise = new Channel(channelVersion, tabSeg[i]);
			//AJOUT DE MON CLIENT AUX MEMBRES ACTIFS DE LA CHANNEL ACTIVE QU'IL VIENT DE CREER
			channelise->addMember(my_client->getNickname());
			//INSERT DE NV CHAN + NV CLIENT A CHANLIST (DA BIG ONE)
			chanList.insert(std::make_pair(channelise, true));
		}
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
#include "../includes/Command.hpp"
#include "../includes/Channel.hpp"
#include <algorithm>
#include <string>

void Command::join()
{
	std::string backToBuf;
    if (!command_leftovers.empty())
	{
		Server *server = my_client->getMyServer();
		std::map<Channel *, bool> chanList = server->GetChannelList();

		chanList = parsingJoin(command_leftovers, chanList);
		std::string nameChan;
		for (std::map<Channel *, bool>::iterator it = chanList.begin(); it != chanList.end(); it++)
		{
			nameChan = it->first->getNameChannel();
		}
	
		server->addChannelList(chanList);
		response_buffer.append(":" + my_client->getNickname() + "!" + my_client->getUsername() + "@localhost JOIN :" + nameChan + "\r\n");
		response_buffer.append(":@localhost 353 " + my_client->getUsername() + " = " + nameChan + " :" + my_client->getNickname() + "\r\n");
		response_buffer.append(":@localhost 366 " + my_client->getUsername() + " " + nameChan + " :End of /NAMES list.");
		is_ready = true;
		is_not_accepted = false;
		setConcernedClients(my_client->getNickname());

		status = SINGLE_SEND;
		// std::cout << std::endl;
		// std::cout << "After join :" << std::endl;
		// displayChannelAndMember();
		// std::cout << std::endl;
	}
}



std::map<Channel *, bool> Command::parsingJoin(std::string command_leftovers, std::map<Channel *, bool> chanList)
{
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
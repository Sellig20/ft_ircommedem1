#include "../includes/Command.hpp"
#include "../includes/Channel.hpp"
#include <algorithm>
#include <string>
#include <cerrno>

/**
 * @brief modifications fanny :
 * 		- break dans parsingJoin() pour bug multiplication des channels
 * 		- ajout de give_back_all_users_name pour le cas ou ya plusieurs utilisateurs dans le channel, histoire d'avoir tous les noms des users de ce channel lors de la connexion du nouvel user
 * 		- envoie d'un message general a tous les utilisateurs du channel en cas de nouvelle connexion
 */
std::string give_back_all_users_names(std::vector<std::string> members_of_chans, std::string channel_name)
{
	std::string all_names = " " + channel_name + " : ";
	for (size_t i = 0; i < members_of_chans.size(); i++)
	{
		if (i < members_of_chans.size() - 1)
			all_names.append(members_of_chans[i] + " ");
		else
			all_names.append(members_of_chans[i]);
	}
	return all_names;
}

void Command::send_arrived_to_all_channel(std::string nameChan, Client *my_client, std::string all_names)
{
	std::string send_welcome_to_all("PRIVMSG " + nameChan + " :" + my_client->getNickname() + " have join the channel");
	Command *priv_to_all_of_chan = new Command(send_welcome_to_all, my_client);
	std::string _buffer_to_send;
	for (size_t i = 0; i < priv_to_all_of_chan->getConcernedClients().size(); i++)
	{
		Client *desti = my_client->getMyServer()->find_destination(priv_to_all_of_chan->getConcernedClients()[i]);
		if (desti == NULL)
			continue ;
		if (priv_to_all_of_chan->getIs_ready() == true)
		{
			_buffer_to_send = ":" + my_client->getNickname();
			_buffer_to_send += " PRIVMSG ";
			_buffer_to_send += priv_to_all_of_chan->getResponseBuffer();
			_buffer_to_send += "\r\n";
			_buffer_to_send.append(":@localhost 353 " + my_client->getNickname() + " =" + all_names + "\r\n");
			_buffer_to_send.append(":@localhost 366 " + my_client->getNickname() + " " + nameChan + " :End of /NAMES list.\r\n");
			while (true)
			{
				int bytesSent = send(desti->GetClientSocketFD(), _buffer_to_send.c_str(), _buffer_to_send.size(), 0);
				if (bytesSent == -1)
				{
					if (errno == EAGAIN || errno == EWOULDBLOCK)
						usleep(10000); 
					else
					{
						perror("Erreur lors de l'envoi :");
						break;
					}
				}
				else
				{
					break;
				}
			}
		}
	}
	delete priv_to_all_of_chan;
}

void Command::join()
{
	std::string backToBuf;
    if (!command_leftovers.empty())
	{
		Server *server = my_client->getMyServer();
		std::map<Channel *, bool> chanList = server->GetChannelList();

		chanList = parsingJoin(command_leftovers, chanList);

		std::vector<std::string> tabSeg;
		std::istringstream ss(command_leftovers);
		std::string seg;
		while (std::getline(ss, seg, ','))
		{
			tabSeg.push_back(seg);
			
		}
		std::string nameChan;
		std::string all_names;
		int i = 0;
		for (std::map<Channel *, bool>::iterator it = chanList.begin(); it != chanList.end(); it++)
		{
			nameChan = it->first->getNameChannel();
			if (nameChan == tabSeg[i])
			{
				//on a trouve le channel concerne par la commande
				all_names = give_back_all_users_names(it->first->getMemberOfThisChan(), it->first->getNameChannel());
				response_buffer.append(":" + my_client->getNickname() + "!" + my_client->getUsername() + "@localhost JOIN :" + nameChan + "\r\n");
				response_buffer.append(":@localhost 353 " + my_client->getNickname() + " =" + all_names + "\r\n");
				response_buffer.append(":@localhost 366 " + my_client->getNickname() + " " + nameChan + " :End of /NAMES list.\r\n");
				send_arrived_to_all_channel(nameChan, my_client, all_names);
				i++;
				it = chanList.begin();
			}
		}
		server->addChannelList(chanList);
		is_ready = true;
		is_not_accepted = false;
		setConcernedClients(my_client->getNickname());
		status = SINGLE_SEND;
	}
}



std::map<Channel *, bool> Command::parsingJoin(std::string command_leftovers, std::map<Channel *, bool> chanList)
{
	int count = 0;
	std::string firstchan;
	std::string secondChan;

	int j = 0;
	char buffer[20];
	size_t i = 0;

	std::vector<std::string> tabSeg;
	std::istringstream ss(command_leftovers);
	std::string seg;
	while (std::getline(ss, seg, ','))
	{
		tabSeg.push_back(seg);
		count += 1;
		// std::cout << ">>>>>>>>>>>>>>>>> " << seg << std::endl;
	}
	// std::cout << "SIZE DE TABSEG = " << tabSeg.size() << std::endl;
	while (i < tabSeg.size() && j < count)
	{
		// std::cout << "tabSeg en cours " << tabSeg[i] << std::endl;
		if (!chanList.empty())
		{
			for (std::map<Channel*, bool>::iterator it = chanList.begin(); it != chanList.end(); it++)
			{
				if (it->first->getNameChannel() == tabSeg[i] && it->first->getMemberOfThisChan().size() == MAX_MEMBERS_PER_CHANNEL)
				{
					std::cerr << "CHANNEL = " << tabSeg[i] << " is FULL" << std::endl;
					break ;
				}
				if (it->first->getNameChannel() == tabSeg[i] && it->first->getMemberOfThisChan().size() < MAX_MEMBERS_PER_CHANNEL) //si tu trouves la channel, juste ajouter le client dedans
				{
					it->first->addMember(my_client->getNickname());
					break ;
				}
				else if (chanList.size() < MAX_CHANNELS) //sinon creation de la channel et ajout du client dedans
				{
					std::cerr << "CHANNEL = " << tabSeg[i] << " has been CREATED" << std::endl;
					sprintf(buffer, "%d", j);
					std::string channelVersion = "Channel_";
					channelVersion.append(buffer);
					Channel *channelise = new Channel(channelVersion, tabSeg[i]);
					channelise->addMember(my_client->getNickname());
					chanList.insert(std::make_pair(channelise, true));
					break ;
				}
				else if (chanList.size() == MAX_CHANNELS)
				{
					std::cerr << "CHANNEL ARE ALL FULL" << std::endl;
					continue ;
				}
				else 
				{
					
					continue ;
				}
			}
		}
		else if (chanList.size() < MAX_CHANNELS)
		{
			std::cerr << "CHANNEL = " << tabSeg[i] << " has been CREATED" << std::endl;
			// std::cerr << "CHANLIST IS EMPTY AND ChAnLIST tOO" << std::endl;
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
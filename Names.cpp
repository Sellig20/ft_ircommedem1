#include "../includes/Command.hpp"

void Command::names()
{
	Server *server = my_client->getMyServer();
	std::map<Channel *, bool> &chanList = server->GetChannelList();
	std::string full_buffer_client;
	
	std::vector<std::string> tabSeg;
	std::istringstream ss(command_leftovers);
	std::string seg;

	while (std::getline(ss, seg, ','))
		tabSeg.push_back(seg);
	if (command_leftovers.size() == 0)
	{
		int socket = my_client->GetClientSocketFD();
		std::string full_buffer_client = Concerned_Buffers[socket];
		std::vector<Client *> activeUser;
		activeUser = GetRegisteredClients();

		std::map<Channel *, bool>::iterator it = chanList.begin();
		full_buffer_client.append(":" + my_client->getMyServer()->GetServerName() + " 353 " + my_client->getNickname() + " = " + it->first->getNameChannel() + " :" + my_client->getNickname() + "\r\n");
		full_buffer_client.append(":" + my_client->getMyServer()->GetServerName() + " 366 " + my_client->getNickname() + " " + it->first->getNameChannel() + " :End of /NAMES list.\r\n");
		full_buffer_client.append(" [");
		for (size_t i = 0; i < activeUser.size(); i++)
		{
			full_buffer_client.append(activeUser[i]->getNickname());
			if (i < activeUser.size() - 1)
				full_buffer_client.append(",");
		}
		full_buffer_client.append("]");
		full_buffer_client.append(" :End of /NAMES\r\n");
		Concerned_Buffers[socket] = full_buffer_client;
		setStatus(NOT_ALL_SEND);
		return ;
	}

	size_t pos = 0;
	std::vector<Client *> activeUser;
	activeUser = GetRegisteredClients();
	while (pos < tabSeg.size())
	{
		size_t i = 0;
		bool flagCondition = false;
		for (std::map<Channel *, bool>::iterator it = chanList.begin(); it != chanList.end(); it++)
		{
			std::vector<std::string> activeChannel;
			activeChannel = getActiveChan();
			while (i < activeChannel.size())
			{
				if (it->second == true && command_leftovers == activeChannel[i])
				{
					int socket = my_client->GetClientSocketFD();
					std::string full_buffer_client = Concerned_Buffers[socket];
					std::vector<std::string> activeUser;
					activeUser = it->first->getMemberOfActiveChan(my_client, chanList, command_leftovers);
					full_buffer_client.append(":" + my_client->getMyServer()->GetServerName() + " 353 " + my_client->getNickname() + " = " + it->first->getNameChannel() + " :" + my_client->getNickname() + "\r\n");
					full_buffer_client.append(":" + my_client->getMyServer()->GetServerName() + " 366 " + my_client->getNickname() + " " + it->first->getNameChannel() + " :End of /NAMES list.\r\n");
					full_buffer_client.append(" [");
					for (size_t i = 0; i < activeUser.size(); i++)
					{
						full_buffer_client.append(activeUser[i]);
						if (i < activeUser.size() - 1)
							full_buffer_client.append(",");
					}
					full_buffer_client.append("]");
					full_buffer_client.append(" :End of /NAMES\r\n");
					Concerned_Buffers[socket] = full_buffer_client;
					setStatus(NOT_ALL_SEND);
					flagCondition = true;
					break ;
				}
				else if (command_leftovers != it->first->getNameChannel())
				{
					it++;
				}
				i++;
			}
			if (flagCondition == false)
			{
				int socket = my_client->GetClientSocketFD();
				std::string full_buffer_client = Concerned_Buffers[socket];
				full_buffer_client.append(":" + my_client->getMyServer()->GetServerName() + " 403 " + my_client->getNickname() + " " + it->first->getNameChannel() + " :No suouich channel");
				Concerned_Buffers[socket] = full_buffer_client;
				setStatus(NOT_ALL_SEND);
				return ;
			}
		}
		pos++;
	}
}
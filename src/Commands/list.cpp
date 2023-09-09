#include "../includes/Command.hpp"



void Command::list()
{
	Server *server = my_client->getMyServer();
	std::map<Channel *, bool> &chanList = server->GetChannelList();
	std::string full_buffer_client;
	std::vector<std::string> activeChan = getActiveChan();

	std::vector<std::string> tabSeg;
	std::istringstream ss(command_leftovers);
	std::string seg;
	
	while (std::getline(ss, seg, ','))
	{
		tabSeg.push_back(seg);
	}

	if (command_leftovers.size() == 0) //LIST -YES
	{
		int socket = my_client->GetClientSocketFD();
		std::string full_buffer_client = Concerned_Buffers[socket];
		full_buffer_client.append(":" + my_client->getMyServer()->GetServerName() + " " + my_client->getNickname());
		full_buffer_client.append(" [");
		for (size_t i = 0; i < activeChan.size(); i++)
		{
			full_buffer_client.append(activeChan[i]);
			if (i < activeChan.size() - 1)
				full_buffer_client.append(",");
		}
		full_buffer_client.append("]");
		full_buffer_client.append(" :End of /LIST\r\n");
		Concerned_Buffers[socket] = full_buffer_client;
		// is_ready = true;
		setStatus(NOT_ALL_SEND);
		return ;
	}

	size_t pos = 0;
	std::map<Channel *, bool>::iterator it;
	while (pos < tabSeg.size())
	{
		for (it = chanList.begin(); it != chanList.end(); it++)
		{
			std::cout << "command elft = " << command_leftovers << " and it->first->cham = " << it->first->getNameChannel() << std::endl;
			if (it->first->getNameChannel() == command_leftovers && it->second == true)//LIST #chan1
			{
				int socket = my_client->GetClientSocketFD();
				std::string full_buffer_client = Concerned_Buffers[socket];
				full_buffer_client.append(":" + my_client->getMyServer()->GetServerName() + " " + my_client->getNickname());
				full_buffer_client.append(" [" + it->first->getNameChannel() + "] :End of /LIST\r\n");
				Concerned_Buffers[socket] = full_buffer_client;
				// is_ready = true;
				setStatus(NOT_ALL_SEND);
				break ;
			}
			else if (command_leftovers[0] == '>' && it->second == true) //LIST >3
			{
				command_leftovers.erase(0, 1);
				long unsigned int parsing = std::atoi(command_leftovers.c_str());
				std::vector<std::string> activeChan = getNumActiveChan(parsing);
				int socket = my_client->GetClientSocketFD();
				std::string full_buffer_client = Concerned_Buffers[socket];
				full_buffer_client.append(":" + my_client->getMyServer()->GetServerName() + " " + my_client->getNickname());
				full_buffer_client.append(" [");
				for (size_t i = 0; i < activeChan.size(); i++)
				{
					full_buffer_client.append(activeChan[i]);
					if (i < activeChan.size() - 1)
						full_buffer_client.append(",");
				}
				full_buffer_client.append("]");
				full_buffer_client.append(" :End of /LIST\r\n");
				Concerned_Buffers[socket] = full_buffer_client;
				// is_ready = true;
				setStatus(NOT_ALL_SEND);
				break ;
			}
		}
		if (it == chanList.end())
		{
			int socket = my_client->GetClientSocketFD();
			std::string full_buffer_client = Concerned_Buffers[socket];
			full_buffer_client.append(":" + my_client->getMyServer()->GetServerName() + " 403 " + my_client->getNickname() + " " + it->first->getNameChannel() + " :No such channel");
			Concerned_Buffers[socket] = full_buffer_client;
			break ;
		}
		pos++;
	}
}
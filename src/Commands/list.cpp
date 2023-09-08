#include "../includes/Command.hpp"

std::vector<std::string> Command::getNumActiveChan(long unsigned int parsing)
{
	Server *server = my_client->getMyServer();
	std::map<Channel *, bool> &chanList = server->GetChannelList();

	std::vector<std::string> res;

	for (std::map<Channel *, bool>::iterator it = chanList.begin(); it != chanList.end(); it++)
	{
		if (it->second == true && it->first->getMemberOfThisChan().size() >= parsing + 1)
		{
			res.push_back(it->first->getNameChannel());
		}
	}
	return res;
}

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
		is_ready = true;
		setStatus(NOT_ALL_SEND);
		return ;
	}

	size_t pos = 0;
	while (pos < tabSeg.size())
	{
		for (std::map<Channel *, bool>::iterator it = chanList.begin(); it != chanList.end(); it++)
		{
			if (command_leftovers == it->first->getNameChannel() && it->second == true)//LIST #chan1
			{
				int socket = my_client->GetClientSocketFD();
				std::string full_buffer_client = Concerned_Buffers[socket];
				full_buffer_client.append(":" + my_client->getMyServer()->GetServerName() + " " + my_client->getNickname());
				full_buffer_client.append(" [" + it->first->getNameChannel() + "] :End of /LIST\r\n");
				Concerned_Buffers[socket] = full_buffer_client;
				is_ready = true;
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
				is_ready = true;
				setStatus(NOT_ALL_SEND);
				break ;
			}
			else
			{
				int socket = my_client->GetClientSocketFD();
				std::string full_buffer_client = Concerned_Buffers[socket];
				full_buffer_client.append(":" + my_client->getMyServer()->GetServerName() + " 403 " + my_client->getNickname() + " " + it->first->getNameChannel() + " :No such channel");
				Concerned_Buffers[socket] = full_buffer_client;
				break ;
			}
		}
		pos++;
	}
}

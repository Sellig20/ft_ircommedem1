#include "../includes/Command.hpp"

void Command::names()
{
	Server *server = my_client->getMyServer();
	std::map<Channel *, bool> &chanList = server->GetChannelList();
	std::string full_buffer_client;
	
	std::vector<std::string> tabSeg;
	std::istringstream ss(command_leftovers);
	std::string seg;
	while (std::getline(ss, seg, ' '))
	{
		tabSeg.push_back(seg);
	}
	if (tabSeg.empty())
	{
		for (std::map<Channel*, bool>::iterator it = chanList.begin(); it != chanList.end(); it++)
		{
			if (it->second == false)
				break;
			else
			{
				std::vector<std::string> memberOfThisChan = it->first->getMemberOfThisChan();
				long unsigned int i = 0;
				std::string result;
				full_buffer_client.append(":" + my_client->getMyServer()->GetServerName() + my_client->getNickname());
				while (i < memberOfThisChan.size())
				{
					result += "NAMES for1 ";
					result += it->first->getNameChannel();
					result += " : ";
					result += memberOfThisChan[i];
					full_buffer_client.append(result);
					full_buffer_client.append("\r\n");
					i++;
				}
				std::string channel = it->first->getNameChannel();
				int socket = my_client->GetClientSocketFD();
				std::string full_buffer_client = Concerned_Buffers[socket];
				full_buffer_client.append("NAMES " + channel + " :End of /NAMES list\r\n");
				Concerned_Buffers[socket] = full_buffer_client;
				is_ready = true;
				setStatus(NOT_ALL_SEND);
			}
		}
	}
	else
	{
		for (std::map<Channel*, bool>::iterator it = chanList.begin(); it != chanList.end(); it++)
		{
			std::vector<std::string>::iterator itArgNames = tabSeg.begin();
			while (itArgNames != tabSeg.end())
			{
				if (it->first->getNameChannel() == *itArgNames)
				{
					std::vector<std::string> sample = it->first->getMemberOfThisChan();
					std::string result;
					full_buffer_client.append(":" + my_client->getMyServer()->GetServerName() + my_client->getNickname());
					for (std::vector<std::string>::iterator i = sample.begin(); i != sample.end(); i++)
					{
						result += "NAMES for2 ";
						result += it->first->getNameChannel();
						result += " : ";
						result += *i;
						full_buffer_client.append(result);
						full_buffer_client.append("\r\n");
					}
				}
				else
				{
					int socket = my_client->GetClientSocketFD();
					std::string full_buffer_client = Concerned_Buffers[socket];
					full_buffer_client.append(":" + my_client->getMyServer()->GetServerName() + " 403 " + my_client->getNickname() + " " + it->first->getNameChannel() + " :No such channel");
					Concerned_Buffers[socket] = full_buffer_client;
					is_ready = true;
					setStatus(NOT_ALL_SEND);
				}
				itArgNames++;
			}
			std::string channel = it->first->getNameChannel();
			int socket = my_client->GetClientSocketFD();
			std::string full_buffer_client = Concerned_Buffers[socket];
			full_buffer_client.append("NAMES " + channel + " :End of /NAMES list \r\n");
			Concerned_Buffers[socket] = full_buffer_client;
			is_ready = true;
			setStatus(NOT_ALL_SEND);
			break ;
		}
	}
}
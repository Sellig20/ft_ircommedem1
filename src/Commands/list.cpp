#include "../includes/Command.hpp"

void Command::list()
{
	Server *server = my_client->getMyServer();
	std::map<Channel *, bool> &chanList = server->GetChannelList();
	// displayChannelAndMember(chanList);
	for (std::map<Channel*, bool>::iterator it = chanList.begin(); it != chanList.end(); it++)
	{
		if (command_leftovers.size() == 0 && it->second == true)//si c'est /LIST
		{
			std::vector<std::string> sample = it->first->getMemberOfThisChan();
			std::string result;
			// response_buffer.append("# " + it->first->getNameChannel());
			full_buffer_client.append(":" + my_client->getMyServer()->GetServerName() + my_client->getNickname());
			for (std::vector<std::string>::iterator i = sample.begin(); i != sample.end(); i++)
			{
				result += "(";
				result += *i;
				result += ")\r\n";
				response_buffer.append(result);
			}
			full_buffer_client.append(it->first->getNameChannel() + " :End of /LIST\r\n");
			Concerned_Buffers[socket] = full_buffer_client;
			is_ready = true;
			setStatus(NOT_ALL_SEND);
		}
		else if (command_leftovers[0] == '>' && it->second == true)//si c'est LIST > 3
		{
			command_leftovers.erase(0, 1);
			long unsigned int count = std::atoi(command_leftovers.c_str());
			if (it->first->getMemberOfThisChan().size() >= count + 1)
			{
				std::vector<std::string> sample = it->first->getMemberOfThisChan();
				std::string result;
				int socket = my_client->GetClientSocketFD();
				std::string full_buffer_client = Concerned_Buffers[socket];
				full_buffer_client.append(":" + my_client->getMyServer()->GetServerName() + my_client->getNickname());
				// response_buffer.append("# " + it->first->getNameChannel());
				for (std::vector<std::string>::iterator i = sample.begin(); i != sample.end(); i++)
				{
					result += "(";
					result += *i;
					result += ") \r\n";
					full_buffer_client.append(result);
				}
				full_buffer_client.append(it->first->getNameChannel() + " :End of /LIST\r\n");
				Concerned_Buffers[socket] = full_buffer_client;
				is_ready = true;
				setStatus(NOT_ALL_SEND);
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
	}

}

#include "../includes/Command.hpp"

int Command::numerousActiveChan(int allActiveChannel, long unsigned int parsing)
{
	Server *server = my_client->getMyServer();
	std::map<Channel *, bool> &chanList = server->GetChannelList();
	std::vector<std::string> numerousActiveVectorChan = getNumActVecChan();

	for (std::map<Channel*, bool>::iterator it = chanList.begin(); it != chanList.end(); it++)
	{
		if (it->second == true && it->first->getMemberOfThisChan().size() >= parsing + 1)
		{
			allActiveChannel += 1;
			std::string sample = it->first->getNameChannel();
			numerousActiveVectorChan.push_back(sample);
		}
	}
	// std::cout << ">>>>>>>>>>>>>> active chan = " << allActiveChannel << std::endl;
	return allActiveChannel;
}

void Command::list()
{
	Server *server = my_client->getMyServer();
	std::map<Channel *, bool> &chanList = server->GetChannelList();
	std::string full_buffer_client;
	for (std::map<Channel*, bool>::iterator it = chanList.begin(); it != chanList.end(); it++)
	{
		if (command_leftovers.size() == 0 && it->second == true)//si c'est /LIST
		{
			std::vector<std::string> sample = it->first->getMemberOfThisChan();
			std::string result;
			full_buffer_client.append(":" + my_client->getMyServer()->GetServerName() + my_client->getNickname());
			for (std::vector<std::string>::iterator i = sample.begin(); i != sample.end(); i++)
			{
				result += "(";
				result += *i;
				result += ")\r\n";
				response_buffer.append(result);
			}
			std::string channel = it->first->getNameChannel();
			int socket = my_client->GetClientSocketFD();
			std::string full_buffer_client = Concerned_Buffers[socket];
			full_buffer_client.append(it->first->getNameChannel() + " :End of /LIST\r\n");
			Concerned_Buffers[socket] = full_buffer_client;
			is_ready = true;
			setStatus(NOT_ALL_SEND);
		}
		else if (command_leftovers == it->first->getNameChannel())
		{
			std::vector<std::string> sample = it->first->getMemberOfThisChan();
			std::string result;
			full_buffer_client.append(":" + my_client->getMyServer()->GetServerName() + my_client->getNickname());
			result += "(";
			result += command_leftovers;
			result += ")\r\n";
			response_buffer.append(result);
			std::string channel = it->first->getNameChannel();
			int socket = my_client->GetClientSocketFD();
			std::string full_buffer_client = Concerned_Buffers[socket];
			full_buffer_client.append(it->first->getNameChannel() + " :End of /LIST\r\n");
			Concerned_Buffers[socket] = full_buffer_client;
			is_ready = true;
			setStatus(NOT_ALL_SEND);
		}
		else if (command_leftovers[0] == '>' && it->second == true)//si c'est LIST > 3
		{
			command_leftovers.erase(0, 1);
			long unsigned int count = std::atoi(command_leftovers.c_str());
			int allActiveChannel = numerousActiveChan(allActiveChannel, count);
			if (it->first->getMemberOfThisChan().size() >= count + 1)
			{
				// std::string sample = it->first->getNameChannel();
				// std::string result;
				int socket = my_client->GetClientSocketFD();
				std::string full_buffer_client = Concerned_Buffers[socket];
				full_buffer_client.append(":" + my_client->getMyServer()->GetServerName() + my_client->getNickname());
				// for (std::vector<std::string>::iterator i = sample.begin(); i != sample.end(); i++)
				// {
				// 	result += "(";
				// 	result += *i;
				// 	result += ") \r\n";
				// 	full_buffer_client.append(result);
				// }
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

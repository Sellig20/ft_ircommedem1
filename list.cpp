#include "../includes/Command.hpp"

void Command::list()
{
	Server *server = my_client->getMyServer();
	std::map<Channel *, bool> &chanList = server->GetChannelList();
	displayChannelAndMember(chanList);
	for (std::map<Channel*, bool>::iterator it = chanList.begin(); it != chanList.end(); it++)
	{
		if (command_leftovers.size() == 0 && it->second == true)
		{
			std::vector<std::string> sample = it->first->getMemberOfThisChan();
			std::string result;
			response_buffer.append("# " + it->first->getNameChannel());
			for (std::vector<std::string>::iterator i = sample.begin(); i != sample.end(); i++)
			{
				result += "(";
				result += *i;
				result += ")";
				response_buffer.append(result);
			}
			is_ready = true;
		//	is_not_accepted = false;
			setConcernedClients(my_client->getNickname());
			status = SINGLE_SEND;
		}
		else if (command_leftovers[0] == '>' && it->second == true)
		{
			command_leftovers.erase(0, 1);
			long unsigned int count = std::atoi(command_leftovers.c_str());
			if (it->first->getMemberOfThisChan().size() >= count + 1)
			{
				std::vector<std::string> sample = it->first->getMemberOfThisChan();
				std::string result;
				response_buffer.append("# " + it->first->getNameChannel());
				for (std::vector<std::string>::iterator i = sample.begin(); i != sample.end(); i++)
				{
					result += "(";
					result += *i;
					result += ")";
					response_buffer.append(result);
				}
				is_ready = true;
			//	is_not_accepted = false;
				setConcernedClients(my_client->getNickname());
				status = SINGLE_SEND;
			}
		}
		else
		{

		}
	}

}

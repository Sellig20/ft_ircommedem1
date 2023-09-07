#include "../includes/Command.hpp"

void Command::names()
{
	Server *server = my_client->getMyServer();
	std::map<Channel *, bool> &chanList = server->GetChannelList();
	
	std::vector<std::string> tabSeg;
	std::istringstream ss(command_leftovers);
	std::string seg;
	while (std::getline(ss, seg, ' '))
	{
		tabSeg.push_back(seg);
	}
	for (std::map<Channel*, bool>::iterator it = chanList.begin(); it != chanList.end(); it++)
	{
		std::vector<std::string>::iterator iti = tabSeg.begin();
		while (iti != tabSeg.end())
		{
			if (it->first->getNameChannel() == *iti)
			{
				std::vector<std::string> sample = it->first->getMemberOfThisChan();
				std::string result;
				for (std::vector<std::string>::iterator i = sample.begin(); i != sample.end(); i++)
				{
					result += "- ";
					result += *i;
					response_buffer.append(result);
				}
			}
			iti++;
		}
		std::string channel = it->first->getNameChannel();
		response_buffer.append("NAMES " + my_client->getNickname() + " " + channel + " :End of /NAMES list \r\n");
		is_ready = true;
	//	is_not_accepted = false;
		setConcernedClients(my_client->getNickname());
		status = SINGLE_SEND;
		break ;
	}
}

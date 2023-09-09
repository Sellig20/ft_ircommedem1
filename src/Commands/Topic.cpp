#include "../includes/Command.hpp"
#include "../includes/Channel.hpp"

void Command::topic()
{
	std::cout << "++++++++++++ je suis dans topic voici le leftorvers : " << command_leftovers << " +++++++++++++" << std::endl;

	Server *server = my_client->getMyServer();
	std::map<Channel *, bool> &chanList = server->GetChannelList();

	std::cout << std::endl;
	std::cout << "in TOPIC " << std::endl;

	std::vector<std::string> tabSeg;
	std::vector<std::string> tabSave;
	std::istringstream ss(command_leftovers);
	std::string seg;
	for (std::map<Channel*, bool>::iterator it = chanList.begin(); it != chanList.end(); it++)
	{
		if (it->second == false)
			break;
		else
		{
			while (std::getline(ss, seg, ' '))
			{
				tabSeg.push_back(seg);
			}
			if ((tabSeg.size() == 1) && (!it->first->getTopicChannel().empty()))
			{
				response_buffer.append("Checking the TOPIC for " + it->first->getNameChannel() + " : " + it->first->getTopicChannel() + "\r\n");
			}
			else if (tabSeg.size() > 1 && tabSeg[1][0] == ':')
			{
				if (tabSeg[1].size() > 1)
				{
					tabSeg[1].erase(0, 1);
					for (size_t i = 1; i < tabSeg.size(); i++)
						tabSave.push_back(tabSeg[i]);
					it->first->setTopicChannel(builtReasonWhy(tabSave));
					error_code = "!";
					is_ready = true;
					response_buffer.append("Setting the TOPIC on " + it->first->getNameChannel() + " to " + it->first->getTopicChannel() + "\r\n");
					// is_not_accepted = false;
					setConcernedClients(my_client->getNickname());
					status = SINGLE_SEND;
				}
				else
				{
					//need to clear topic
					it->first->setTopicChannel("NULL");
					response_buffer.append("Clearing the TOPIC on " + it->first->getNameChannel() + "\r\n");
					is_ready = true;
					// is_not_accepted = false;
					setConcernedClients(my_client->getNickname());
					status = SINGLE_SEND;
				}
			}
			else
			{
				response_buffer.append(my_client->getNickname() + " " + it->first->getNameChannel() + " :No topic set \r\n");
				is_ready = true;
				// is_not_accepted = false;
				setConcernedClients(my_client->getNickname());
				status = SINGLE_SEND;
			}
		}
	}
	std::cout << std::endl;
}
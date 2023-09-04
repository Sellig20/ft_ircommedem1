#include "../includes/Command.hpp"
#include "../includes/Channel.hpp"

void Command::topic()
{
	std::cout << "++++++++++++ je suis dans topic voici le leftorvers : " << command_leftovers << " +++++++++++++" << std::endl;

	Server *server = my_client->getMyServer();
	std::map<Channel *, bool> &chanList = server->GetChannelList();

	std::cout << std::endl;
	std::cout << "in TOPIC " << std::endl;
	displayChannelAndMember(chanList);

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
				std::cout << "TOPIC SIZE == 1" << std::endl;
				response_buffer.append("Checking the topic for " + it->first->getNameChannel() + " : " + it->first->getTopicChannel() + "\r\n");
			}
			else if (tabSeg.size() > 1 && tabSeg[1][0] == ':')
			{
				std::cout << "{{{{{{{{{ " << tabSeg << std::endl;
				if (tabSeg[1].size() > 1)
				{
					std::cout << "TAB 1 sup a 1" << std::endl;
					tabSeg[1].erase(0, 1);
					it->first->setTopicChannel(builtReasonWhy(tabSeg));
					error_code = "!";
					response_buffer.append("Setting the topic on " + it->first->getNameChannel() + " to " + it->first->getTopicChannel() + "\r\n");
					std::cout << "BONJOUR BNJOUR BONJOUR" << std::endl;
					is_ready = true;
					// is_not_accepted = false;
					setConcernedClients(my_client->getNickname());
					status = SINGLE_SEND;
				}
				else
				{
					//need to clear topic
					it->first->setTopicChannel("NULL");
					response_buffer.append("Clearing the topic on " + it->first->getNameChannel() + "\r\n");
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

//   TOPIC #test :New topic          ; Setting the topic on "#test" to
//                                   "New topic".

//   TOPIC #test :                   ; Clearing the topic on "#test"

//   TOPIC #test                     ; Checking the topic for "#test"

#include "../includes/Command.hpp"

void Command::list()
{
	std::cout << "++++++++++++ je suis dans list voici le leftorvers : " << command_leftovers << " +++++++++++++" << std::endl;
	
	Server *server = my_client->getMyServer();
	std::map<Channel *, bool> &chanList = server->GetChannelList();
	displayChannelAndMember(chanList);
	for (std::map<Channel*, bool>::iterator it = chanList.begin(); it != chanList.end(); it++)
	{
		std::cout << "Channel enregistreeeeeeeeees ---------> " << it->first->getNameChannel() << std::endl;
		// for (size_t iti = 0; iti != it->first->getMemberOfThisChan().size(); iti++)
		// {
			std::cout << it->first->getMemberOfThisChan() << std::endl;
		// }
	}

}


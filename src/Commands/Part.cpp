#include "../includes/Command.hpp"
#include "../includes/Channel.hpp"


void Command::part()
{
	std::cout << "++++++++++++ je suis dans part voici le leftorvers : " << command_leftovers << " +++++++++++++" << std::endl;
	
	
	Server *server = my_client->getMyServer();
	std::map<Channel *, bool> chanList = server->GetChannelList();
	
	for (std::map<Channel *, bool>::iterator it = chanList.begin(); it != chanList.end(); it++)
	{
		std::cout << "!!! /PART ==> Creation de " << it->first->getNumChannel() << " | nom = " << it->first->getNameChannel() << " par " << my_client->getUsername() << std::endl;
		std::cout << "Member of this channel : " << it->first->getMemberOfThisChan() << std::endl;
	}

}

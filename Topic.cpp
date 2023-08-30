#include "../includes/Command.hpp"
#include "../includes/Channel.hpp"

void Command::topic()
{
	std::cout << "++++++++++++ je suis dans topic voici le leftorvers : " << command_leftovers << " +++++++++++++" << std::endl;
	// std::cout << "dans topic" << std::endl;

	// Server *server = my_client->getMyServer();
	// std::map<Channel *, bool> chanList = server->GetChannelList();

	// for (std::map<Channel *, bool>::iterator it = chanList.begin(); it != chanList.end(); it++)
	// {
	// 	std::cout << "!!! /JOIN ==> Creation de " << it->first->getNumChannel() << " | nom = " << it->first->getNameChannel() << std::endl;
	// }
}
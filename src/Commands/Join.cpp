#include "../includes/Command.hpp"
#include "../includes/Channel.hpp"

void Command::join()
{
	std::cout << "++++++++++++ je suis dans join voici le leftorvers : " << command_leftovers << "+++++++++++++" << std::endl;
    if (!command_leftovers.empty())
	{
		Channel *chan = new Channel(my_client);
		chan->setNameChannel(command_leftovers);
		std::cout << "Channel name = " << chan->getNameChannel() << std::endl;
		delete chan;
	}
}

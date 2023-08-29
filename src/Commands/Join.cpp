#include "../includes/Command.hpp"
#include "../includes/Channel.hpp"

void Command::join()
{
	std::string backToBuf;
	int nbChan(0);
	std::cout << "++++++++++++ je suis dans join voici le leftovers : " << command_leftovers << "+++++++++++++" << std::endl;
    if (!command_leftovers.empty())
	{
		Channel *chan = new Channel(my_client);
		Server *server = my_client->getMyServer();
		std::map<Channel *, bool> chanList = server->GetChannelList();

		nbChan = parsingJoin(command_leftovers, chan, chanList);
		// std::cout << "nomb re de chan = " << nbChan << std::endl;
		// chan->setNameChannel(command_leftovers);
		// std::cout << "Channel name = " << chan->getNameChannel() << std::endl;
		(void)nbChan;
		// backToBuf.append(":" + my_client->getNickname() + "!" + my_client->getNickname() + "@localhost JOIN " + chan->getNameChannel() + "\n");
		// backToBuf.append(":" + my_client.getNickname() + "!" + my_client.getNickname() + "localhost 332 " + my_client.getNickname() + " " + chan->getNameChannel() + " :" + channel.getTopicChannel() + "\n"); 
		delete chan;
	}
}

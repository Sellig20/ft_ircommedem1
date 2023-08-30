#include "../includes/Channel.hpp"
#include "../includes/Client.hpp"


Channel::Channel()
{
	// clientListForChan();
}

Channel::~Channel()
{

}

Channel::Channel(std::string num, std::string name)
{
	numChannel = num;
	nameChannel = name;
}

const std::string Channel::getNumChannel()
{
	return numChannel;
}

void Channel::setNameChannel(std::string nameToBe)
{
	nameChannel = nameToBe;
}

const std::string Channel::getNameChannel()
{
	return nameChannel;
}

Channel::Channel(Client *my_client)
{
	Server *tmp = my_client->getMyServer();
	std::vector<Client *> tmp_connectedClients = tmp->GetRegisteredClients();
	for (std::vector<Client *>::iterator it = tmp_connectedClients.begin(); it != tmp_connectedClients.end(); it++)
	{
		Client *my_g = *it;
		std::string dataNickname = my_g->getNickname();
		std::cout << my_g << std::endl;
		_clientListForChan.insert(std::make_pair(my_g, dataNickname));
	}
	for(std::map<Client*, std::string>::iterator k = _clientListForChan.begin(); k != _clientListForChan.end(); k++)
	{
		std::cout << "map => [" << k->first << " ," << k->second << "]" << std::endl;
	}
}

#include "../includes/Channel.hpp"

Channel::Channel()
{
	// clientListForChan();
}

Channel::~Channel()
{

}

Channel::Channel(Client *my_client)
{
	Server *tmp = my_client->getMyServer();
	std::vector<Client *> tmp_connectedClients = tmp->GetConnectedClient();
	// int i = 0;
	for (std::vector<Client *>::iterator it = tmp_connectedClients.begin(); it != tmp_connectedClients.end(); it++)
	{
	    std::cout << "Adresse -> " << *it << std::endl;
		Client *my_g = *it;
		std::string dataNickname = my_g->getNickname();
		std::cout << my_g << std::endl;
		_clientListForChan.insert(std::make_pair(my_g, dataNickname));
	}
	for(std::map<Client*, std::string>::iterator k = _clientListForChan.begin(); k != _clientListForChan.end(); k++)
	{
		std::cout << "map => " << k->second << std::endl;
	}

}

	// while (tmp_connectedClients[i])
	// {
	//     // std::cout << "tmp_connectedClients[" << i << "] = " << tmp_connectedClients[i]->getNickname() << std::endl;
	// 	i++;
	// }


	// std::cout << "????? " << gg.front() << std::endl;
	// Client *my_c = getNickname();
	// std::cout << "?????????????????? " << my_c << std::endl;



	// for (int i(0); connected_clients[i]; i++)
	// {
	// 	std::cout << "Connected_clients[" << i << "] = " << tmp.connected_clients[i]->getNickname() << std::endl;
	// 	_clientListForChan.insert(std::make_pair(connected_clients[i].getNickname(), &client));
	// }

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

void Channel::setNameChannel(std::string nameToBe)
{
	nameChannel = nameToBe;
}

void Channel::setTopicChannel(std::string topicToBe)
{
	topicChannel = topicToBe;
}

const std::string Channel::getNameChannel()
{
	return nameChannel;
}

const std::string Channel::getNumChannel()
{
	return numChannel;
}

const std::string Channel::getTopicChannel()
{
	return topicChannel;
}

const std::vector<std::string> Channel::getMemberOfThisChan()
{
	return memberOfThisChan;
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

bool Channel::isMember(std::string memberName)
{
	for(size_t i = 0; i != memberOfThisChan.size(); i++)
	{
		if (memberOfThisChan[i] == memberName)
			return true;
		else
			return false;
	}
	return false;
}

void Channel::addMember(const std::string& member)
{
	memberOfThisChan.push_back(member);
}

std::ostream& operator<<(std::ostream& o, const std::vector<std::string>& memberVector)
{
	o << "[";
	for (size_t i(0); i < memberVector.size(); i++)
	{
		o << memberVector[i];
		if (i < memberVector.size() - 1)
			o << ", ";
	}
	o << "]";
	return o;
}
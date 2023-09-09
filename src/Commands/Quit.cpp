#include "../includes/Command.hpp"

/*
	ERROR CODE = pas de cpde
*/


void Command::quit()
{	
	broadcast_to_all_channels_user_is_in(my_client);
	status = NOT_ALL_SEND;
	std::map<Channel *, bool>::iterator it  = my_client->getMyServer()->GetChannelList().begin();
	while (it != my_client->getMyServer()->GetChannelList().end())
	{
		if (it->first->getMemberOfThisChan().size() == 0)
		{
			delete it->first;
			my_client->getMyServer()->GetChannelList().erase(it++);
			if (it == my_client->getMyServer()->GetChannelList().end() && my_client->getMyServer()->GetChannelList().size() == 0)
			{
				my_client->getMyServer()->GetChannelList().clear();
			}
		}
		else
			++it;
	}
}
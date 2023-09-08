#include "../includes/Command.hpp"


void Command::broadcast_to_all_channels_user_is_in(Client *my_client)
{
	std::string nameChan, all_names;

	std::string newbie_buffer = ":" + my_client->getNickname() + "!" + my_client->getUsername() + "@" + my_client->getMyServer()->GetServerName() + " QUIT :Quit:he wanted to leave\r\n";
	int socket = my_client->GetClientSocketFD();
	std::string full_buffer_client = Concerned_Buffers[socket];
	full_buffer_client.append(newbie_buffer);
	Concerned_Buffers[socket] = full_buffer_client;
	for (std::map<Channel *, bool>::iterator it = my_client->getMyServer()->GetChannelList().begin(); it != my_client->getMyServer()->GetChannelList().end(); it++)
	{
		nameChan = it->first->getNameChannel();
		for (size_t i = 0; i < my_client->getMyServer()->send_back_channel(nameChan)->getMemberOfThisChan().size(); i++)
		{
			if (my_client->getNickname() != my_client->getMyServer()->send_back_channel(nameChan)->getMemberOfThisChan()[i])
			{
				std::string my_buffer = ":" + my_client->getNickname() + "!" + my_client->getUsername() + "@" + my_client->getMyServer()->GetServerName() + " QUIT :Quit:he wanted to leave\r\n";
				my_buffer += ":" + my_client->getMyServer()->GetServerName() + " PRIVMSG " + nameChan + " :" + my_client->getNickname() + " quit the channel\r\n";
				int socket = my_client->getMyServer()->find_destination(my_client->getMyServer()->send_back_channel(nameChan)->getMemberOfThisChan()[i])->GetClientSocketFD();
				std::string full_buffer_client = Concerned_Buffers[socket];
				full_buffer_client.append(my_buffer);
				Concerned_Buffers[socket] = full_buffer_client;
			}
		}
		if (my_client->getMyServer()->send_back_channel(nameChan)->getMemberOfThisChan().size() > 0)
		{
			std::vector<std::string> new_list = kick_user(my_client->getMyServer()->send_back_channel(nameChan)->getMemberOfThisChan(), my_client->getNickname());
			my_client->getMyServer()->send_back_channel(nameChan)->setMemberOfthisChan(new_list);
		}
	}
	std::map<Channel *, bool>::iterator it = my_client->getMyServer()->GetChannelList().begin();
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

void Command::quit()
{	
	is_not_accepted = true;
	my_client->setRequestCode("999");
	error_code = command_leftovers;
	// response_buffer = "999: " + my_client->getNickname() + " has left the server, we forget all about him";
	is_ready = true;
	broadcast_to_all_channels_user_is_in(my_client);
	status = NOT_ALL_SEND;


	
}
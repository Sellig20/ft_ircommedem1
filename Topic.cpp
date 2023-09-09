#include "../includes/Command.hpp"
#include "../includes/Channel.hpp"

/*
	ERROR CODES TOPIC :
	461 -> need more params
	403 -> no such channel
	442 -> user not on channel
	482 -> you are not operator

*/

std::string Command::builtReasonWhy(std::vector<std::string> toReason)
{
	std::string sample;
	size_t j = 0;

	while (j < toReason.size())
	{
		sample.append(toReason[j]);
		sample.append(" ");
		j++;
	}
	return (sample);
}

void Command::send_topic_updates_to_all_members_of_chan(Client *my_client, std::string channel, std::string content)
{
	(void)content;
	for (size_t i = 0; i < my_client->getMyServer()->send_back_channel(channel)->getMemberOfThisChan().size(); i++)
	{
		if (my_client->getMyServer()->is_my_client_registered(my_client->getMyServer()->send_back_channel(channel)->getMemberOfThisChan()[i]) == true )
		{
			std::string my_buffer = ":" + my_client->getNickname() + "@"  + my_client->getMyServer()->GetServerName() + " TOPIC " + channel + " " + my_client->getMyServer()->send_back_channel(channel)->getTopicChannel() + "\r\n";
			int socket = my_client->getMyServer()->find_destination(my_client->getMyServer()->send_back_channel(channel)->getMemberOfThisChan()[i])->GetClientSocketFD();
			std::string full_buffer_client = Concerned_Buffers[socket];
			full_buffer_client.append(my_buffer);
			Concerned_Buffers[socket] = full_buffer_client;
		}
	}

}

void Command::topic()
{
	if (command_leftovers.empty())
	{
		fill_error_need_more_params(this);
		return ;
	}
	std::string channel, deux_points, topick;
	int mode = 0;
	if (command_leftovers.find(" ") > command_leftovers.size())
	{
		channel = command_leftovers;
	}
	else
	{
		channel = command_leftovers.substr(0, command_leftovers.find(" "));
		deux_points = command_leftovers[channel.size() + 1];
		topick = command_leftovers.substr(channel.size() + 2, command_leftovers.size());
	}
	if (deux_points.empty())
		mode = 0;
	else if (topick.empty())
		mode = 1;
	else if (!topick.empty())
		mode = 2;
	setStatus(NOT_ALL_SEND);
	if (my_client->getMyServer()->is_channel_on_server(channel) == false)
	{
		//403
		int socket = my_client->GetClientSocketFD();
		std::string full_buffer_client = Concerned_Buffers[socket];
		full_buffer_client.append(":@"  + my_client->getMyServer()->GetServerName() + " 403 " + channel + " " + channel + "\r\n");
		Concerned_Buffers[socket] = full_buffer_client;
		return ;
	}
	else if (my_client->getMyServer()->is_member_of_this_chan(channel, my_client->getNickname()) == false)
	{
		//442
		int socket = my_client->GetClientSocketFD();
		std::string full_buffer_client = Concerned_Buffers[socket];
		full_buffer_client.append(":" + my_client->getNickname() + "@"  + my_client->getMyServer()->GetServerName() + " 442 " + my_client->getNickname() + " :You are not on channel [" +  channel + "]\r\n");
		Concerned_Buffers[socket] = full_buffer_client;
		return ;
	}
	else if (my_client->getMyServer()->send_back_channel(channel)->getTopicRestrictionMode() == true && my_client->getMyServer()->send_back_channel(channel)->is_member_operator(my_client->getNickname()) == false)
	{
		//482
		int socket = my_client->GetClientSocketFD();
		std::string full_buffer_client = Concerned_Buffers[socket];
		full_buffer_client.append(":" + channel + "@"  + my_client->getMyServer()->GetServerName() + " 482 " +  channel + " :you are not channel operator\r\n");
		Concerned_Buffers[socket] = full_buffer_client;
		return ;
	}
	if (mode == 0)
	{
		//display topic but irssi does that himself
		int socket = my_client->GetClientSocketFD();
		std::string full_buffer_client = Concerned_Buffers[socket];
		full_buffer_client.append(":" + my_client->getNickname() + "@"  + my_client->getMyServer()->GetServerName() + " Checking the TOPIC for " + channel + " : " + channel + "\r\n");
		Concerned_Buffers[socket] = full_buffer_client;
	}
	else if (mode == 1)
	{
		//clear topic
		my_client->getMyServer()->send_back_channel(channel)->setTopicChannel("");
		for (size_t i = 0; i < my_client->getMyServer()->send_back_channel(channel)->getMemberOfThisChan().size(); i++)
		{
			if (my_client->getMyServer()->is_my_client_registered(my_client->getMyServer()->send_back_channel(channel)->getMemberOfThisChan()[i]) == true )
			{
				std::string my_buffer = ":" + my_client->getNickname() + "@"  + my_client->getMyServer()->GetServerName() + " TOPIC " + channel + " :No topic is set\r\n";
				int socket = my_client->getMyServer()->find_destination(my_client->getMyServer()->send_back_channel(channel)->getMemberOfThisChan()[i])->GetClientSocketFD();
				std::string full_buffer_client = Concerned_Buffers[socket];
				full_buffer_client.append(my_buffer);
				Concerned_Buffers[socket] = full_buffer_client;
			}
		}
	}
	else if (mode == 2)
	{
		//change topic
		my_client->getMyServer()->send_back_channel(channel)->setTopicChannel(topick);
		send_topic_updates_to_all_members_of_chan(my_client, channel, topick);
	}
}
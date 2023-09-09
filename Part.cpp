#include "../includes/Command.hpp"
#include "../includes/Channel.hpp"
#include <string>
#include <algorithm>


/* ERROR PART
	461 need more params ok
	403 no such channel
	442 not on channel
*/

void Command::part()
{
	if (command_leftovers.empty())
	{
		fill_error_need_more_params(this);
		return ;
	}
	if (my_client->getMyServer()->GetChannelList().empty())
	{
		//403
		int socket = my_client->GetClientSocketFD();
		std::string full_buffer_client = Concerned_Buffers[socket];
		full_buffer_client.append(":" + my_client->getNickname() + "@"  + my_client->getMyServer()->GetServerName() + " 403 " + my_client->getNickname() + " :" + command_leftovers + "\r\n");
		Concerned_Buffers[socket] = full_buffer_client;
		// is_ready = true;
		setStatus(NOT_ALL_SEND);
		return ;
	}
	std::vector<std::string> _channels;
	std::string reasonWhy, channel_names, tmp;
	channel_names = command_leftovers.substr(0, command_leftovers.find(" "));
	if (channel_names.size() < command_leftovers.size())
		reasonWhy = command_leftovers.substr(command_leftovers.find(" "), command_leftovers.size());
	std::istringstream iss(channel_names);
	while (std::getline(iss, tmp, ','))
	{
		_channels.push_back(tmp);
	}
	status = NOT_ALL_SEND;
	for (size_t j = 0; j < _channels.size(); j++)
	{
		if (my_client->getMyServer()->is_channel_on_server(_channels[j]) == true)
		{
			//on a trouve le bon chan
			if (my_client->getMyServer()->is_member_of_this_chan(_channels[j], my_client->getNickname()) == true)
			{
				//membre bien present dans le channel
				int socket = my_client->GetClientSocketFD();
				std::string full_buffer_client = Concerned_Buffers[socket];
				std::string buffer = (":" + my_client->getNickname() + "!" + my_client->getUsername() + "@" + my_client->getMyServer()->GetServerName() +" PART " + _channels[j] + " " + _channels[j] + "\r\n");
				full_buffer_client.append(buffer);
				Concerned_Buffers[socket] = full_buffer_client;
				
				std::vector<std::string> new_list = kick_user(my_client->getMyServer()->send_back_channel(_channels[j])->getMemberOfThisChan(), my_client->getNickname());
				my_client->getMyServer()->send_back_channel(_channels[j])->setMemberOfthisChan(new_list);
				for (size_t i = 0; i < new_list.size(); i++)
				{
					if (my_client->getMyServer()->is_my_client_registered(new_list[i]) == true)
					{
						std::string buffer = ":" + my_client->getNickname() + "@" + my_client->getMyServer()->GetServerName() + " PRIVMSG " + _channels[j] + " :" + my_client->getNickname() + " has left the channel";
						if (!reasonWhy.empty())
							buffer.append(" because " + reasonWhy + "\r\n");
						else
							buffer.append("\r\n");
						int socket = my_client->getMyServer()->find_destination(new_list[i])->GetClientSocketFD();
						std::string full_buffer_client = Concerned_Buffers[socket];
						full_buffer_client.append(buffer);
						Concerned_Buffers[socket] = full_buffer_client;
					}

				}
			}
			else
			{
				//442
				int socket = my_client->GetClientSocketFD();
				std::string full_buffer_client = Concerned_Buffers[socket];
				full_buffer_client.append(":" + my_client->getNickname() + "@"  + my_client->getMyServer()->GetServerName() + " 442 " + my_client->getNickname() + " :You are not on channel [" + _channels[j] + "]\r\n");
				Concerned_Buffers[socket] = full_buffer_client;
				// is_ready = true;
			}
		}
		else
		{
			//403
			int socket = my_client->GetClientSocketFD();
			std::string full_buffer_client = Concerned_Buffers[socket];
			full_buffer_client.append(":" + my_client->getNickname() + "@"  + my_client->getMyServer()->GetServerName() + " 403 " + my_client->getNickname() + " :" + _channels[j] + "\r\n");
			Concerned_Buffers[socket] = full_buffer_client;
			// is_ready = true;
		}

	}
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
	return ;
}
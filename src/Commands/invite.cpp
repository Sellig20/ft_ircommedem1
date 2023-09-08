#include "../includes/Command.hpp"

void Command::invite()
{
	std::istringstream iss(command_leftovers);
	std::string target, channel;
	
	iss >> target >> channel;
	if (target.empty() || channel.empty())
	{
		fill_error_need_more_params(this);
		return ;
	}
	if (my_client->getMyServer()->is_channel_on_server(channel) == true)
	{
		if (my_client->getMyServer()->is_member_of_this_chan(channel, my_client->getNickname()) ==  true)
		{
			if (my_client->getMyServer()->is_my_client_registered(target) == false)
			{
				fill_no_such_nick();
				return ;
			}
			else
			{
				int socket = my_client->GetClientSocketFD();
				std::string full_buffer_client = Concerned_Buffers[socket];
				full_buffer_client.append(":" + my_client->getNickname() + "@"  + my_client->getMyServer()->GetServerName() + " 341 " + my_client->getNickname() + " " + target + " " + channel + "\r\n");
				Concerned_Buffers[socket] = full_buffer_client;
				socket = my_client->getMyServer()->find_destination(target)->GetClientSocketFD();
				std::string newbie_buffer = ":" + my_client->getNickname() + "!" + my_client->getUsername() + "@" + my_client->getMyServer()->GetServerName() + " INVITE " + target + " " + channel + "\r\n";
				my_client->getMyServer()->send_back_channel(channel)->addInviteMembers(target);
				Concerned_Buffers[socket] = newbie_buffer;
				is_ready = true;
				setStatus(NOT_ALL_SEND);
				return ;
			}
		}
		else if (my_client->is_member_of_this_chan(channel, target) == true)
		{
			int socket = my_client->GetClientSocketFD();
			std::string full_buffer_client = Concerned_Buffers[socket];
			full_buffer_client.append(":" + my_client->getNickname() + "@"  + my_client->getMyServer()->GetServerName() + " 443 " + my_client->getNickname() + " :" + target + " is already a member of channel [" + channel + "]\r\n");
			Concerned_Buffers[socket] = full_buffer_client;
			is_ready = true;
			setStatus(NOT_ALL_SEND);
			return ;
		}
		else
		{
			int socket = my_client->GetClientSocketFD();
			std::string full_buffer_client = Concerned_Buffers[socket];
			full_buffer_client.append(":" + my_client->getNickname() + "@"  + my_client->getMyServer()->GetServerName() + " 442 " + my_client->getNickname() + " :You are not on channel [" + channel + "]\r\n");
			Concerned_Buffers[socket] = full_buffer_client;
			is_ready = true;
			setStatus(NOT_ALL_SEND);
			return ;
		}
	}
	else
	{
		int socket = my_client->GetClientSocketFD();
		std::string full_buffer_client = Concerned_Buffers[socket];
		full_buffer_client.append(":" + my_client->getNickname() + "@"  + my_client->getMyServer()->GetServerName() + " 403 " + my_client->getNickname() + " :" + channel + "\r\n");
		Concerned_Buffers[socket] = full_buffer_client;
		is_ready = true;
		setStatus(NOT_ALL_SEND);
	}
}


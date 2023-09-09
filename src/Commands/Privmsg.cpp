#include "../includes/Command.hpp"

/*
 ERROR CODES PRIVMSG :
	401 : no such nick
	404 : cannot send to chan
	411 : no content
*/

void Command::privmsg()
{
	if (command_leftovers.empty())
	{
		fill_error_need_more_params(this);
		return ;
	}
	std::string expediteur, content, dest, rest;
	std::istringstream iss(command_leftovers);
	
	iss >> dest;
	std::getline(iss, rest, ':');
	std::getline(iss, content);
	if (content.empty())
	{
		//412
		int socket = my_client->GetClientSocketFD();
		std::string full_buffer_client = Concerned_Buffers[socket];
		full_buffer_client.append(":" + my_client->getNickname() + "@"  + my_client->getMyServer()->GetServerName() + " 412 " + my_client->getNickname() + " :no text to send\r\n");
		Concerned_Buffers[socket] = full_buffer_client;
		setStatus(NOT_ALL_SEND);
		return ;
	}
	else if (content.size() > 512)
	{
		//417
		int socket = my_client->GetClientSocketFD();
		std::string full_buffer_client = Concerned_Buffers[socket];
		full_buffer_client.append(":" + my_client->getNickname() + "@"  + my_client->getMyServer()->GetServerName() + " 417 " + my_client->getNickname() + " :input is too long\r\n");
		Concerned_Buffers[socket] = full_buffer_client;
		setStatus(NOT_ALL_SEND);
		return ;
	}
	if (dest[0] == '#' && !content.empty())
	{
		for (std::map<Channel *, bool>::iterator it = my_client->getMyServer()->GetChannelList().begin(); it != my_client->getMyServer()->GetChannelList().end(); it++)
		{
			if (it->first->getNameChannel() == dest && my_client->getMyServer()->is_member_of_this_chan(dest, my_client->getNickname()) == true)
			{
				for (size_t i = 0; i < my_client->getMyServer()->send_back_channel(dest)->getMemberOfThisChan().size(); i++)
				{
					if (my_client->getNickname() != my_client->getMyServer()->send_back_channel(dest)->getMemberOfThisChan()[i] && my_client->getMyServer()->is_my_client_registered(my_client->getMyServer()->send_back_channel(dest)->getMemberOfThisChan()[i]) == true )
					{
						std::string my_buffer = ":" + my_client->getNickname() + "!" + my_client->getUsername() + "@" + my_client->getMyServer()->GetServerName() + " PRIVMSG " + dest + " :" + content +"\r\n";
						int socket = my_client->getMyServer()->find_destination(my_client->getMyServer()->send_back_channel(dest)->getMemberOfThisChan()[i])->GetClientSocketFD();
						std::string full_buffer_client = Concerned_Buffers[socket];
						full_buffer_client.append(my_buffer);
						Concerned_Buffers[socket] = full_buffer_client;
					}
				}
			}
			else if (my_client->getMyServer()->is_member_of_this_chan(dest, my_client->getNickname()) == false)
			{
				//442
				int socket = my_client->GetClientSocketFD();
				std::string full_buffer_client = Concerned_Buffers[socket];
				full_buffer_client.append(":" + my_client->getNickname() + "@"  + my_client->getMyServer()->GetServerName() + " 442 " + my_client->getNickname() + " :You are not on channel [" + dest + "]\r\n");
				Concerned_Buffers[socket] = full_buffer_client;
				// is_ready = true;
			}

			else
			{
				//404
				int socket = my_client->GetClientSocketFD();
				std::string full_buffer_client = Concerned_Buffers[socket];
				full_buffer_client.append(":" + my_client->getNickname() + "@"  + my_client->getMyServer()->GetServerName() + " 403 " + my_client->getNickname() + " :" + command_leftovers + "\r\n");
				Concerned_Buffers[socket] = full_buffer_client;
				setStatus(NOT_ALL_SEND);
				return ;
			}
		}
		if (Concerned_Buffers.size() > 0)
			status = NOT_ALL_SEND;
		else
			fill_no_such_nick();
	}
	else
	{
		Client *destinataire = my_client->getMyServer()->find_destination(dest);
		if (destinataire == NULL)
		{
			//401
			fill_no_such_nick();
		}
		else
		{
			std::string my_buffer = ":" + my_client->getNickname() + " PRIVMSG " + dest + " :" + content +"\r\n";
			int socket = my_client->getMyServer()->find_destination(dest)->GetClientSocketFD();
			std::string full_buffer_client = Concerned_Buffers[socket];
			full_buffer_client.append(my_buffer);
			Concerned_Buffers[socket] = full_buffer_client;
			status = NOT_ALL_SEND;

		}
	}

}

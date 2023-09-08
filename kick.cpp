#include "../includes/Command.hpp"
#include <cerrno>


// void Server::is_channel_on_service(std::string channel_name)
// {

// }


void Command::kick()
{
	// std::cout << "++++++++++++ je suis dans kick voici le leftorvers : " << command_leftovers << " +++++++++++++" << std::endl;
	//<<KICK #bot truc :truc

	std::istringstream iss(command_leftovers);
	// std::vector<std::string> output;
	std::string channel_from, target, reason;
	
	iss >> channel_from >> target;
	if (channel_from.empty())
	{
		fill_error_need_more_params(this);
		return ;
	}
	if (my_client->GetIsOper() ==  false)
	{
		int socket = my_client->GetClientSocketFD();
		std::string full_buffer_client = Concerned_Buffers[socket];
		full_buffer_client.append(":" + channel_from + "@"  + my_client->getMyServer()->GetServerName() + " 482 " + channel_from + " :you are not channel operator\r\n");
		Concerned_Buffers[socket] = full_buffer_client;
		is_ready = true;
		setStatus(NOT_ALL_SEND);
		return ;
	}
	reason = command_leftovers.substr(channel_from.size() + target.size () + 3, command_leftovers.size());

	if (my_client->getMyServer()->is_channel_on_server(channel_from) == true)
	{
		if (my_client->getMyServer()->is_member_of_this_chan(channel_from, target) == true)
		{
			if (my_client->getMyServer()->send_back_channel(channel_from)->getMemberOfThisChan().size() > 0)
			{
				std::vector<std::string> new_list = kick_user(my_client->getMyServer()->send_back_channel(channel_from)->getMemberOfThisChan(), target);
				my_client->getMyServer()->send_back_channel(channel_from)->setMemberOfthisChan(new_list);
			}
			is_not_accepted = true;
			my_client->setRequestCode("");
			error_code = "";
			int socket = my_client->getMyServer()->find_destination(target)->GetClientSocketFD();



			std::string full_buffer_client = Concerned_Buffers[socket];
			full_buffer_client.append( ":" + target + "!" + target + "@" + my_client->getMyServer()->GetServerName() + " PART " + channel_from + " " + channel_from + "\r\n");
			if (!reason.empty())
				full_buffer_client.append(":" + my_client->getMyServer()->GetServerName() + " You have been kicked from channel because " + reason + "\r\n");
			else
				full_buffer_client.append(":" + my_client->getMyServer()->GetServerName() + " You have been kicked from channel because we dont need reason\r\n");

			Concerned_Buffers[socket] = full_buffer_client;
			is_ready = true;
			setStatus(NOT_ALL_SEND);
			std::string all_names = give_all_names(my_client->getMyServer()->send_back_channel(channel_from)->getMemberOfThisChan(), channel_from);
			if (my_client->getMyServer()->send_back_channel(channel_from)->getMemberOfThisChan().size() > 0)
			{
				for (size_t i = 0; i < my_client->getMyServer()->send_back_channel(channel_from)->getMemberOfThisChan().size(); i++)
				{
					if (my_client->getMyServer()->send_back_channel(channel_from)->getMemberOfThisChan()[i] != target)
					{
						// std::cerr << "ADDING => " << my_client->getMyServer()->send_back_channel(channel_from)->getMemberOfThisChan()[i] << std::endl;
						std::string my_buffer = ":" + my_client->getMyServer()->GetServerName() + " PRIVMSG " + channel_from + " :" + target + " has been kicked out of the channel\r\n";
						my_buffer.append(":" + my_client->getMyServer()->GetServerName() + " 353 " + my_client->getMyServer()->send_back_channel(channel_from)->getMemberOfThisChan()[i] + " = " + channel_from + " :" + all_names + "\r\n");
						// if (!reason.empty())
						my_buffer.append(":" + my_client->getMyServer()->GetServerName() + " 366 " +  my_client->getMyServer()->send_back_channel(channel_from)->getMemberOfThisChan()[i] + " " + channel_from + " :End of /NAMES list.\r\n");
						int socket = my_client->getMyServer()->find_destination(my_client->getMyServer()->send_back_channel(channel_from)->getMemberOfThisChan()[i])->GetClientSocketFD();
						std::string full_buffer_client = Concerned_Buffers[socket];
						full_buffer_client.append(my_buffer);
						Concerned_Buffers[socket] = full_buffer_client;
					}
				}
			}
			
		}
		else
		{
			int socket = my_client->GetClientSocketFD();
			std::string full_buffer_client = Concerned_Buffers[socket];
			full_buffer_client.append( ":" + channel_from + "@" + my_client->getMyServer()->GetServerName() + " 441 " + channel_from  +  " :user not on channel\r\n");
			Concerned_Buffers[socket] = full_buffer_client;
			is_ready = true;
			setStatus(NOT_ALL_SEND);

		}
	}

	else
	{
			is_not_accepted = true;
			setConcernedClients(my_client->getNickname());
			my_client->setRequestCode("403");
			error_code = "403";
			response_buffer = channel_from + " :no such channel on server";
			is_ready = true;
			setStatus(SINGLE_SEND);
	}


	// std::cout << "asking the kick [" << channel_from << "] target of the kick = [" << target << "] and reason = [" << reason << "]" << std::endl;
}


#include "../includes/Command.hpp"

void Command::names()
{
	Server *server = my_client->getMyServer();
	std::map<Channel *, bool> &chanList = server->GetChannelList();
	std::string full_buffer_client;
	
	std::vector<std::string> tabSeg;
	std::istringstream ss(command_leftovers);
	std::string seg;

	while (std::getline(ss, seg, ','))
	{
		tabSeg.push_back(seg);
		std::cout << "SEG ========>" << seg << std::endl;
	}

	// newbie_buffer = ":" + my_client->getNickname() + "!" + my_client->getUsername() + "@" + my_client->getMyServer()->GetServerName() + " JOIN :" + nameChan + "\r\n";
	// newbie_buffer.append(":" + my_client->getMyServer()->GetServerName() + " 353 " + my_client->getNickname() + " = " + nameChan + " :" + client_nick + "\r\n");
	// newbie_buffer.append(":" + my_client->getMyServer()->GetServerName() + " 366 " + my_client->getNickname() + " " + nameChan + " :End of /NAMES list.\r\n");
	
	if (command_leftovers.size() == 0)
	{
		int socket = my_client->GetClientSocketFD();
		std::string full_buffer_client = Concerned_Buffers[socket];
		std::vector<Client *> activeUser;
		activeUser = GetRegisteredClients();

		std::map<Channel *, bool>::iterator it = chanList.begin();
		// full_buffer_client.append(":" + my_client->getNickname() + "!" + my_client->getNickname() + my_client->getUsername() + "@" + my_client->getMyServer()->GetServerName() + " NAMES :" + it->first->getNameChannel() + "\r\n");
		full_buffer_client.append(":" + my_client->getMyServer()->GetServerName() + " 353 " + my_client->getNickname() + " = " + it->first->getNameChannel() + " :" + my_client->getNickname() + "\r\n");
		full_buffer_client.append(":" + my_client->getMyServer()->GetServerName() + " 366 " + my_client->getNickname() + " " + it->first->getNameChannel() + " :End of /NAMES list.\r\n");
		full_buffer_client.append(" [");
		for (size_t i = 0; i < activeUser.size(); i++)
		{
			full_buffer_client.append(activeUser[i]->getNickname());
			if (i < activeUser.size() - 1)
				full_buffer_client.append(",");
		}
		full_buffer_client.append("]");
		full_buffer_client.append(" :End of /NAMES\r\n");
		Concerned_Buffers[socket] = full_buffer_client;
		is_ready = true;
		setStatus(NOT_ALL_SEND);
		return ;
	}

	size_t pos = 0;
	std::vector<Client *> activeUser;
	activeUser = GetRegisteredClients();
	while (pos < tabSeg.size())
	{
		size_t i = 0;
		bool flagCondition = false;
		for (std::map<Channel *, bool>::iterator it = chanList.begin(); it != chanList.end(); it++)
		{
			std::vector<std::string> activeChannel;
			activeChannel = getActiveChan();
			while (i < activeChannel.size())
			{
				// std::cout << "////// COMMAND_LEFTOVERS = " << command_leftovers << std::endl;
				// std::cout << "////// IT FIRST GET NAME CHANNEL = " << it->first->getNameChannel() << std::endl;
				// std::cout << "////// IT SECOND = " << it->second << std::endl;
				// std::cout << "////// ACTIVE CHANNEL = " << activeChannel[i] << std::endl;
				if (/*command_leftovers != it->first->getNameChannel() &&*/it->second == true && command_leftovers == activeChannel[i])//LIST #chan1
				{
					std::cout << "if ((((((((((1))))))))))" << std::endl;
					int socket = my_client->GetClientSocketFD();
					std::string full_buffer_client = Concerned_Buffers[socket];
					std::vector<std::string> activeUser;
					activeUser = it->first->getMemberOfActiveChan(my_client);
					std::cout << " CHANNEL => " << command_leftovers << " | " << it->first->getNameChannel() << " ////////:: " << activeUser << std::endl;
					// full_buffer_client.append(":" + my_client->getMyServer()->GetServerName() + " " + my_client->getNickname());
					full_buffer_client.append(":" + my_client->getMyServer()->GetServerName() + " 353 " + my_client->getNickname() + " = " + it->first->getNameChannel() + " :" + my_client->getNickname() + "\r\n");
					full_buffer_client.append(":" + my_client->getMyServer()->GetServerName() + " 366 " + my_client->getNickname() + " " + it->first->getNameChannel() + " :End of /NAMES list.\r\n");
					full_buffer_client.append(" [");
					for (size_t i = 0; i < activeUser.size(); i++)
					{
						full_buffer_client.append(activeUser[i]);
						if (i < activeUser.size() - 1)
							full_buffer_client.append(",");
					}
					full_buffer_client.append("]");
					full_buffer_client.append(" :End of /NAMES\r\n");
					Concerned_Buffers[socket] = full_buffer_client;
					is_ready = true;
					setStatus(NOT_ALL_SEND);
					flagCondition = true;
					break ;
				}
				std::cout << "/////// i ======= " << i << std::endl;
				i++;
			}
			if (flagCondition == false)
			{
				std::cout << "else ((((((((((2))))))))))" << std::endl;
				int socket = my_client->GetClientSocketFD();
				std::string full_buffer_client = Concerned_Buffers[socket];
				full_buffer_client.append(":" + my_client->getMyServer()->GetServerName() + " 403 " + my_client->getNickname() + " " + it->first->getNameChannel() + " :No suouich channel");
				Concerned_Buffers[socket] = full_buffer_client;
				is_ready = true;
				setStatus(NOT_ALL_SEND);
				break ;
				
			}
		}
		pos++;
	}
}

// 	if (tabSeg.empty())
// 	{
// 		for (std::map<Channel*, bool>::iterator it = chanList.begin(); it != chanList.end(); it++)
// 		{
// 			if (it->second == false)
// 				break;
// 			else
// 			{
// 				std::vector<std::string> memberOfThisChan = it->first->getMemberOfThisChan();
// 				long unsigned int i = 0;
// 				std::string result;
// 				int socket = my_client->GetClientSocketFD();
// 				std::string full_buffer_client = Concerned_Buffers[socket];
// 				full_buffer_client.append(":" + my_client->getMyServer()->GetServerName() + my_client->getNickname());
// 				while (i < memberOfThisChan.size())
// 				{
// 					result += "NAMES for1 ";
// 					result += it->first->getNameChannel();
// 					result += " : ";
// 					result += memberOfThisChan[i];
// 					full_buffer_client.append(result);
// 					full_buffer_client.append("\r\n");
// 					i++;
// 				}
// 				std::string channel = it->first->getNameChannel();
// 				full_buffer_client.append("NAMES " + channel + " :End of /NAMES list\r\n");
// 				Concerned_Buffers[socket] = full_buffer_client;
// 				is_ready = true;
// 				setStatus(NOT_ALL_SEND);
// 			}
// 		}
// 	}
// 	else
// 	{
// 		for (std::map<Channel*, bool>::iterator it = chanList.begin(); it != chanList.end(); it++)
// 		{
// 			std::vector<std::string>::iterator itArgNames = tabSeg.begin();
// 			while (itArgNames != tabSeg.end())
// 			{
// 				if (it->first->getNameChannel() == *itArgNames)
// 				{
// 					std::vector<std::string> sample = it->first->getMemberOfThisChan();
// 					std::string result;
// 					full_buffer_client.append(":" + my_client->getMyServer()->GetServerName() + my_client->getNickname());
// 					for (std::vector<std::string>::iterator i = sample.begin(); i != sample.end(); i++)
// 					{
// 						result += "NAMES for2 ";
// 						result += it->first->getNameChannel();
// 						result += " : ";
// 						result += *i;
// 						full_buffer_client.append(result);
// 						full_buffer_client.append("\r\n");
// 					}
// 				}
// 				else
// 				{
// 					int socket = my_client->GetClientSocketFD();
// 					std::string full_buffer_client = Concerned_Buffers[socket];
// 					full_buffer_client.append(":" + my_client->getMyServer()->GetServerName() + " 403 " + my_client->getNickname() + " " + it->first->getNameChannel() + " :No such channel");
// 					Concerned_Buffers[socket] = full_buffer_client;
// 					is_ready = true;
// 					setStatus(NOT_ALL_SEND);
// 				}
// 				itArgNames++;
// 			}
// 			std::string channel = it->first->getNameChannel();
// 			int socket = my_client->GetClientSocketFD();
// 			std::string full_buffer_client = Concerned_Buffers[socket];
// 			full_buffer_client.append("NAMES " + channel + " :End of /NAMES list \r\n");
// 			Concerned_Buffers[socket] = full_buffer_client;
// 			is_ready = true;
// 			setStatus(NOT_ALL_SEND);
// 			break ;
// 		}
// 	}
// }
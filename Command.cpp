
#include "../includes/Command.hpp"
#include "../includes/errors_rfc.hpp"

Command::Command()
{
	_flagPart = 0;
	_flagSpace = 0;
	_flagShouldCloseChan = false;
	_flagIsThereAReason = false;
	_isGivenKeyInJoin = false;
}

Command::~Command()
{

}

void Command::setStatus(int _status)
{
	status = _status;
}


int Command::getStatus (void)
{
	return status;
}

std::map<std::string, std::string> Command::getChanKey()
{
	return _chanKey;
}

std::vector<std::string> Command::getNumActVecChan()
{
	return _numerousActiveVectorChan;
}

	std::map<int, std::string>& Command::getConcernedBuffer() {
			return Concerned_Buffers;
		};
		void Command::setClientsMap(std::map<int, std::string>& _input) {
			Concerned_Buffers = _input;
		};

std::vector<std::string>	Command::getActiveChan()
{
	Server *server = my_client->getMyServer();
	std::vector<std::string> activeChan;
	std::map<Channel *, bool> &chanList = server->GetChannelList();
	for (std::map<Channel*, bool>::iterator it = chanList.begin(); it != chanList.end(); it++)
	{
		if (it->second == true)
			activeChan.push_back(it->first->getNameChannel());
	}
	return activeChan;
}

std::vector<std::string> Command::getNumActiveChan(long unsigned int parsing)
{
	Server *server = my_client->getMyServer();
	std::map<Channel *, bool> &chanList = server->GetChannelList();

	std::vector<std::string> res;

	for (std::map<Channel *, bool>::iterator it = chanList.begin(); it != chanList.end(); it++)
	{
		if (it->second == true && it->first->getMemberOfThisChan().size() >= parsing + 1)
		{
			res.push_back(it->first->getNameChannel());
		}
	}
	return res;
}


void Command::send_to_all_users_from_chan(Client *my_client, std::string nameChan, std::vector<std::string> members)
{
	std::string all_names, newbie_buffer, full_buffer_client;
	int socket = 0;
	newbie_buffer = ":" + my_client->getNickname() + "!" + my_client->getUsername() + "@" + my_client->getMyServer()->GetServerName() + " JOIN :" + nameChan + "\r\n";
	// newbie_buffer.append(":" + my_client->getMyServer()->GetServerName() + " 353 " + my_client->getNickname() + " = " + nameChan + " :" +  give_back_all_users_names(members, nameChan) + "\r\n");
	// newbie_buffer.append(":" + my_client->getMyServer()->GetServerName() + " 366 " + my_client->getNickname() + " " + nameChan + " :End of /NAMES list.\r\n");
	newbie_buffer.append(":" + nameChan + "@"  + my_client->getMyServer()->GetServerName() + " TOPIC " + nameChan + " " + my_client->getMyServer()->send_back_channel(nameChan)->getTopicChannel() + "\r\n");
	socket = my_client->GetClientSocketFD();
	full_buffer_client = Concerned_Buffers[socket];
	full_buffer_client.append(newbie_buffer);
	this->Concerned_Buffers[socket] = full_buffer_client;
	if (members.size() > 0)
	{
			for (size_t i = 0; i < members.size(); i++)
			{
				if (my_client->getNickname() != members[i])
				{
					newbie_buffer = ":" + my_client->getMyServer()->GetServerName() + " PRIVMSG " + nameChan + " :" + my_client->getNickname() + " has arrived\r\n";
					// std::cout << "target = " <<  members[i] << std::endl;
					socket =  my_client->getMyServer()->find_destination(members[i])->GetClientSocketFD();
					full_buffer_client = Concerned_Buffers[socket];
					full_buffer_client.append(newbie_buffer);
					Concerned_Buffers[socket] = full_buffer_client;
				}
			}
	}
}

std::vector<std::string> Command::kick_user(std::vector<std::string> memberOfThisChan, std::string userNickname)
{

	if (!memberOfThisChan.empty())
	{
		for (std::vector<std::string>::iterator it = memberOfThisChan.begin(); it != memberOfThisChan.end(); it++)
		{
			if (*it == userNickname)
			{
				memberOfThisChan.erase(it);
				break ;
			}
		}
	}
	return memberOfThisChan;
}

//est-ce qu'un token contient des white spaces
bool Command::containsOnlySpaces(const std::string& str)
{
	for (size_t i = 0; i < str.length(); ++i)
	{
		if (std::isspace(static_cast<unsigned char>(str[i])))
				return false;
	}
	return true;
}

//pour USER et NICK d'apres la RFC
bool Command::is_token_valid(std::string nick)
{
	if (isdigit(nick[0]))
		return (false);
	else if (!nick.find('#'))
		return (false);
	else if (!nick.find(':'))
		return (false);
	else if (containsOnlySpaces(nick) == false)
		return (false);
	return (true);
}

std::string Command::give_back_all_users_names(std::vector<std::string> members_of_chans, std::string channel_name)
{
	std::string all_names;
	(void)channel_name;
	for (size_t i = 0; i < members_of_chans.size(); i++)
	{
		if (i < members_of_chans.size() - 1)
			all_names.append(members_of_chans[i] + " ");
		else
			all_names.append(members_of_chans[i]);
	}
	return all_names;
}

std::string Command::give_all_names(std::vector<std::string> members_of_chans, std::string channel_name)
{
	std::string all_names = " " + channel_name + " : ";
	for (size_t i = 0; i < members_of_chans.size(); i++)
	{
		if (i < members_of_chans.size() - 1)
			all_names.append(members_of_chans[i] + " ");
		else
			all_names.append(members_of_chans[i]);
	}
	return all_names;
}

std::vector<std::string> Command::split_leftovers_by_comas(std::string &command_leftovers)
{
	std::string seg;
	std::vector<std::string> tabSeg;
	std::istringstream ss(command_leftovers);
	while (std::getline(ss, seg, ','))
	{
		if (seg.find(" ") > seg.size())
			tabSeg.push_back(seg);
		else
		{
			seg = seg.substr(0, seg.find(" "));
			tabSeg.push_back(seg);
		}
	}
	if (command_leftovers.find(" ") < command_leftovers.size())
		command_leftovers = command_leftovers.substr(command_leftovers.find(' ') + 1, command_leftovers.size());
	return tabSeg;
}


Command::Command(std::string &extracted, Client *_my_client)
{
	status = 0;
	_flagPart = 0;
	_flagSpace = 0;
	_flagShouldCloseChan = false;
	_flagIsThereAReason = false;

 	size_t i(0);
	int com1(0);
	std::string com;
	std::string leftover;
	while (i < extracted.length())
	{
		if (isupper(extracted[i]))
		{
			com1 = extracted.find(" ");
			com = extracted.substr(0, com1);
			if (com.length() > 3 && com.length() < 8)
			{
				i += com.length() - 1;
			}
		}
		else if (extracted[i] == ' ' && i + 1)
		{
			com1 = extracted.find(" ");
			leftover = extracted.substr(com1 + 1, extracted.size());
			command_leftovers = leftover;
			i += leftover.length();
		}
		i++;
	}
	my_client = _my_client;
	command_name = com;
	Server *tmp = my_client->getMyServer();
	std::vector<std::string> tmp_compTab = tmp->GetComptab();
	std::vector<void (Command::*)()> tmp_functionTab = tmp->GetFunctionTab();

	int j(0);
	for (std::vector<std::string>::iterator it = tmp_compTab.begin(); it != tmp_compTab.end(); it++)
	{
		if (*it == com)
		{
			tmp_functionTab[j];
			(this->*tmp_functionTab[j])();
			break ;
		}
		j++;
	}
}

void Command::capls()
{
}


void Command::fill_error_need_more_params(Command *my_command)
{
	(void)my_command;
	int socket = my_client->GetClientSocketFD();
	std::string full_buffer_client = Concerned_Buffers[socket];
	full_buffer_client.append(":" + my_client->getNickname() + "!" + my_client->getUsername() + "@" + my_client->getMyServer()->GetServerName() + " 461 : not enough parameters\r\n");
	Concerned_Buffers[socket] = full_buffer_client;
	setStatus(NOT_ALL_SEND);
}

void Command::fill_error_password_mismatch(Command *my_command)
{
	(void)my_command;
	int socket = my_client->GetClientSocketFD();
	std::string full_buffer_client = Concerned_Buffers[socket];
	full_buffer_client.append(":" + my_client->getNickname() + "!" + my_client->getUsername() + "@" + my_client->getMyServer()->GetServerName() + " 464 : wrong password try again to join [" + my_client->getMyServer()->GetServerName() + "]\r\n");
	Concerned_Buffers[socket] = full_buffer_client;
	setStatus(NOT_ALL_SEND);
}

void Command::fill_error_already_registered(void)
{
	int socket = my_client->GetClientSocketFD();
	std::string full_buffer_client = Concerned_Buffers[socket];
	full_buffer_client.append(":" + my_client->getNickname() + "!" + my_client->getUsername() + "@" + my_client->getMyServer()->GetServerName() + " 462 : you are already registered on [" + my_client->getMyServer()->GetServerName() + "]\r\n");
	Concerned_Buffers[socket] = full_buffer_client;
	setStatus(NOT_ALL_SEND);
}

void Command::fill_no_such_nick(void)
{
	int socket = my_client->GetClientSocketFD();
	std::string full_buffer_client = Concerned_Buffers[socket];
	full_buffer_client.append(":" + my_client->getNickname() + "!" + my_client->getUsername() + "@" + my_client->getMyServer()->GetServerName() + " 401 : not suck nick has been found\r\n");
	Concerned_Buffers[socket] = full_buffer_client;
	setStatus(NOT_ALL_SEND);
}

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
			if (my_client->getNickname() != my_client->getMyServer()->send_back_channel(nameChan)->getMemberOfThisChan()[i] && my_client->getMyServer()->is_my_client_registered(my_client->getMyServer()->send_back_channel(nameChan)->getMemberOfThisChan()[i]) == true )
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
}
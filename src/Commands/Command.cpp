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

void Command::setIs_ready(bool _is_ready)
{
	is_ready = _is_ready;
}

bool Command::getIs_ready() const
{
	return is_ready;
}

const std::string Command::getLeftOver() const
{
	return command_leftovers;
}
const std::string Command::getResponseBuffer() const
{
	return response_buffer;
}

const std::string Command::getErrorcode() const
{
	return error_code;
}

std::map<std::string, std::string> Command::getChanKey()
{
	return _chanKey;
}

std::vector<std::string> Command::getNumActVecChan()
{
	return _numerousActiveVectorChan;
}


bool Command::getIs_Not_Accepted() const
{
	return is_not_accepted;
}

void Command::setConcernedClients(const Client *my_client)
{
	concerned_clients.push_back(my_client->getNickname());
}

void Command::setConcernedClients(const std::string _nick)
{
	concerned_clients.push_back(_nick);
}

void Command::setStatus(int _status)
{
	status = _status;
}

std::vector<std::string> const	&Command::getConcernedClients() const
{
	return concerned_clients;

}

int Command::getStatus (void)
{
	return status;
}

const std::string 				Command::getBroadCastBuffer() const
{
	return broadcast_buffer;
}

std::vector<std::string> const	&Command::getBroadCastVector() const
{
	return broadcast_vector;
}
// on extrait ce qu'il y a apres les majuscules, du coup le contenu de la commande
std::string Command::extractAfterUppercase(const std::string& input)
{
    std::string result;
    bool uppercaseFound = false;

    for (size_t i = 0; i < input.length(); ++i)
	{
        if (std::isupper(input[i]))
            uppercaseFound = true;
        else if (uppercaseFound && std::isspace(input[i]))
		{
            result = input.substr(i + 1);
            break;
        }
    }

    return result;
}

void Command::send_to_all_users_from_chan(Client *my_client, std::string nameChan, std::vector<std::string> members)
{
	std::string all_names, newbie_buffer, full_buffer_client;
	int socket = 0;
	newbie_buffer = ":" + my_client->getNickname() + "!" + my_client->getUsername() + "@" + my_client->getMyServer()->GetServerName() + " JOIN :" + nameChan + "\r\n";
	newbie_buffer.append(":" + my_client->getMyServer()->GetServerName() + " 353 " + my_client->getNickname() + " = " + nameChan + " :" +  give_back_all_users_names(members, nameChan) + "\r\n");
	newbie_buffer.append(":" + my_client->getMyServer()->GetServerName() + " 366 " + my_client->getNickname() + " " + nameChan + " :End of /NAMES list.\r\n");
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
					newbie_buffer = ":" + my_client->getMyServer()->GetServerName() + " 353 " + my_client->getNickname() + " = " + nameChan + " :" + give_all_names(members, nameChan) + "\r\n";
					newbie_buffer += ":" + my_client->getMyServer()->GetServerName() + " PRIVMSG " + nameChan + " :" + my_client->getNickname() + " has arrived\r\n";
					std::cout << "target = " <<  members[i] << std::endl;
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
	// std::cout << "to erase member " << userNickname << std::endl;
	// _flagShouldCloseChan = false;
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
	// std::cout << "NEW COMMAND LEFT = " << command_leftovers << std::endl;
	return tabSeg;
}

void Command::displayChannelAndMember(std::map<Channel *, bool> chanList)
{
	std::cout << "------------------------" << std::endl;
	std::cout << "LES CHANNELS OUVERTES : avec nb_channels =  " << chanList.size() << std::endl;
	for (std::map<Channel*, bool>::iterator it = chanList.begin(); it != chanList.end(); it++)
	{
		if (it->second == true)
		{
			std::cout << it->first->getNameChannel() << std::endl;
		}
	}
	std::cout << "------------------------" << std::endl;
	std::cout << "LES CHANNELS FERMEES : " << std::endl;
	for (std::map<Channel*, bool>::iterator it = chanList.begin(); it != chanList.end(); it++)
	{
		if (it->second == false)
		{
			std::cout << it->first->getNameChannel() << std::endl;
		}
	}
	std::cout << "------------------------" << std::endl;
}


Command::Command(std::string &extracted, Client *_my_client)
{
	//extracted = ligne de cnd [NAME + content]
	is_ready = false;
	is_not_accepted = false;
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
		//!!!!!!!!!!!!!1 /USER est transforme en userhost donc ne rentre pas ici a corriger par zanot
		if (isupper(extracted[i]))
		{
			com1 = extracted.find(" ");
			com = extracted.substr(0, com1);
			if (com.length() > 3 && com.length() < 8)
			{
					// std::cout << "Voici la commande :[" << com ;
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
	// if (command_leftovers.find("\r\n"))
		// command_leftovers =  command_leftovers.substr(0, command_leftovers.find("\r\n"));
	my_client = _my_client;
	command_name = com;
	// std::cout << "CMD NAME = " << command_name << " And left = " << command_leftovers << std::endl;
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
	//en vrai on s'en bats les couilles mais au moins on s'arrete pas dessus
	// std::cout << command_leftovers << std::endl;
	// exit(1);
	// std::cout << "on passe ce CAP LS" << std::endl;
	if (command_leftovers == "LS 302")
	{
		response_buffer = "CAP * LS :multi-prefix sasl=PLAIN,EXTERNAL";
		is_not_accepted = false;
		my_client->setRequestCode("");
		is_ready = true;
	}
	else if (command_leftovers == "REQ :multi-prefix")
	{
		response_buffer = "CAP * ACK :multi-prefix";
		is_not_accepted = false;
		my_client->setRequestCode("");
		is_ready = true;
	}
	else if (command_leftovers == "END")
	{
		response_buffer = "001 :";
		is_not_accepted = false;
		my_client->setRequestCode("");
		is_ready = true;
	}
	// else 
}


void Command::fill_error_need_more_params(Command *my_command)
{
	(void)my_command;
	int socket = my_client->GetClientSocketFD();
	std::string full_buffer_client = Concerned_Buffers[socket];
	full_buffer_client.append(":" + my_client->getNickname() + "!" + my_client->getUsername() + "@" + my_client->getMyServer()->GetServerName() + " 461 : not enough parameters\r\n");
	Concerned_Buffers[socket] = full_buffer_client;
	is_ready = true;
	setStatus(NOT_ALL_SEND);
}

void Command::fill_error_password_mismatch(Command *my_command)
{
	(void)my_command;
	int socket = my_client->GetClientSocketFD();
	std::string full_buffer_client = Concerned_Buffers[socket];
	full_buffer_client.append(":" + my_client->getNickname() + "!" + my_client->getUsername() + "@" + my_client->getMyServer()->GetServerName() + " 464 : wrong password try again to join [" + my_client->getMyServer()->GetServerName() + "]\r\n");
	Concerned_Buffers[socket] = full_buffer_client;
	is_ready = true;
	setStatus(NOT_ALL_SEND);
}

void Command::fill_error_already_registered(void)
{
	int socket = my_client->GetClientSocketFD();
	std::string full_buffer_client = Concerned_Buffers[socket];
	full_buffer_client.append(":" + my_client->getNickname() + "!" + my_client->getUsername() + "@" + my_client->getMyServer()->GetServerName() + " 462 : you are already registered on [" + my_client->getMyServer()->GetServerName() + "]\r\n");
	Concerned_Buffers[socket] = full_buffer_client;
	is_ready = true;
	setStatus(NOT_ALL_SEND);
}

void Command::fill_no_such_nick(void)
{
	int socket = my_client->GetClientSocketFD();
	std::string full_buffer_client = Concerned_Buffers[socket];
	full_buffer_client.append(":" + my_client->getNickname() + "!" + my_client->getUsername() + "@" + my_client->getMyServer()->GetServerName() + " 401 : not suck nick has been found\r\n");
	Concerned_Buffers[socket] = full_buffer_client;
	is_ready = true;
	setStatus(NOT_ALL_SEND);
}

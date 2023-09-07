#include "../includes/Command.hpp"
#include "../includes/errors_rfc.hpp"

Command::Command()
{
	_flagPart = 0;
	_flagSpace = 0;
	_flagShouldCloseChan = false;
	_flagIsThereAReason = false;
	_isOperatorMode = false;
	_topicRestrictionMode = false;
	_canalInviteOnlyMode = false;
	_isSuperOperMode = false;
	_limitUserMode = 0;
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

	is_not_accepted = true;
	my_client->setRequestCode("461");
	error_code = "461";
	response_buffer = " :" + command_name + " Not enough parameters";
	is_ready = true;
	setConcernedClients(my_client->getNickname());
	setStatus(SINGLE_SEND);
}

void Command::fill_error_password_mismatch(Command *my_command)
{
	(void)my_command;
	is_not_accepted = true;
	my_client->setRequestCode("464");
	error_code = "464";
	response_buffer = " :Password incorrect";
	is_ready = true;
	if (!my_client->getNickname().empty())
		setConcernedClients(my_client->getNickname());
	setStatus(SINGLE_SEND);
}

void Command::fill_error_already_registered(void)
{
	is_not_accepted = true;
	my_client->setRequestCode("462");
	error_code = "462";
	response_buffer = command_name + " You may not register again";
	is_ready = true;
	setConcernedClients(my_client->getNickname());
	setStatus(SINGLE_SEND);
}

void Command::fill_no_such_nick(void)
{
	is_not_accepted = true;
	my_client->setRequestCode("401");
	error_code = "401";
	response_buffer = command_name + " Not such nick has been found on server";
	is_ready = true;
	setConcernedClients(my_client->getNickname());
	setStatus(SINGLE_SEND);
}

#include "../includes/Command.hpp"
#include "../includes/Channel.hpp"
#include "../includes/errors_rfc.hpp"

Command::Command()
{

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

// bool isS


#include <sstream>
#include <vector>
#include <cstdio>
int Command::parsingJoin(std::string command_leftovers, Channel *chan, std::map<Channel *, bool> chanList)
{
	(void)chan;
	int count = 0;
	std::string firstchan;
	std::string secondChan;
	std::vector<std::string> tabSeg;
	std::istringstream ss(command_leftovers);
	std::string seg;
	int j = 0;
	char buffer[20];
	size_t i = 0;

	while (std::getline(ss, seg, ','))
	{
		tabSeg.push_back(seg);
		count += 1;
	}

	// for (size_t it = 0; it < tabSeg.size(); it++)
	// {
	// 	std::cout << "Token " << it + 1 << " : " << tabSeg[it] << std::endl;
	// }
	while (i < tabSeg.size() && j < count)
	{
			// std::cout << "tab seg de i : " << tabSeg[i] << std::endl;
			sprintf(buffer, "%d", j);
			std::string channelVersion = "Channel_";
			channelVersion.append(buffer);
			Channel * channelise = new Channel(channelVersion, tabSeg[i]);
			chanList.insert(std::make_pair(channelise, true));
			j++;
			i++;
	}
	for (std::map<Channel *, bool>::iterator it = chanList.begin(); it != chanList.end(); it++)
	{
		std::cout << "!!! /JOIN ==> Creation de " << it->first->getNumChannel() << " | nom = " << it->first->getNameChannel() << std::endl;
	}
	return (count);
}

Command::Command(std::string &extracted, Client *_my_client)
{
	is_ready = false;
	is_not_accepted = false;

 	size_t i(0);
	int com1(0);
	std::string com;
	std::string leftover;
	while (i < extracted.length())
	{
		//!!!!!!!!!!!!!1 /USER est transforme en userhost donc ne rentre pas ici a corriger par zanot
		if (isupper(extracted[i]))
		{
			if (extracted == "CAP LS")
			{
				com = extracted;
				std::cout << "Voici la commande : " << com << std::endl;
				i += com.length() - 1;
			}
			else
			{
				com1 = extracted.find(" ");
				com = extracted.substr(0, com1);
				if (com.length() > 3 && com.length() < 8)
				{
					std::cout << "Voici la commande : " << com << std::endl;
					i += com.length() - 1;
				}
				else
					std::cerr << "Commande non-existante. Message anormal" << std::endl;
			}
		}
		else if (extracted[i] == ' ' && i + 1)
		{
			com1 = extracted.find(" ");
			leftover = extracted.substr(com1 + 1, extracted.size());
			command_leftovers = leftover;
			std::cout << "Voici le leftovers : " << leftover << std::endl;
			i += leftover.length();
		}
		i++;
	}
	my_client = _my_client;
	Server *tmp = my_client->getMyServer();
	std::vector<std::string> tmp_compTab = tmp->GetComptab();
	std::vector<void (Command::*)()> tmp_functionTab = tmp->GetFunctionTab();

	int j(0);
	for (std::vector<std::string>::iterator it = tmp_compTab.begin(); it != tmp_compTab.end(); it++)
	{
		if (*it == com)
		{
			// std::cout << "ICI avec com = " << com << std::endl;
			tmp_functionTab[j];
			(this->*tmp_functionTab[j])();
		}
		j++;
	}
	std::cout << std::endl;
	std::cout << std::endl;
}

void Command::capls()
{
	//en vrai on s'en bats les couilles mais au moins on s'arrete pas dessus
	std::cout << "on passe ce CAP LS" << std::endl;
}

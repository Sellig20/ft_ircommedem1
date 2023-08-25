#include "../includes/Command.hpp"

Command::Command()
{

}

Command::~Command()
{

}

Command::Command(std::string com, std::string leftover, Client *my_client)
{
	(void)leftover;

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
		}
		// else
		// 	std::cerr << "NOP DOPE" << std::endl;
		// free-delete ici ?
		j++;
	}
}

void Command::capls()
{
	std::cout << "++++++++++++ je suis dans capls +++++++++++++" << std::endl;
}

void Command::user()
{
	std::cout << "++++++++++++ je suis dans user +++++++++++++" << std::endl;
}

void Command::nick()
{
	std::cout << "++++++++++++ je suis dans nick +++++++++++++" << std::endl;
}

void Command::pass()
{
	std::cout << "++++++++++++ je suis dans pass +++++++++++++" << std::endl;
}

void Command::ping()
{
	std::cout << "++++++++++++ je suis dans ping +++++++++++++" << std::endl;
}

void Command::mode()
{
	std::cout << "++++++++++++ je suis dans mode +++++++++++++" << std::endl;
}

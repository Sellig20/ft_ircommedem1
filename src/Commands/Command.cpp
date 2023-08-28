#include "../includes/Command.hpp"

Command::Command()
{

}

Command::~Command()
{

}

Command::Command(std::string com, std::string leftover, Client *my_client)
{
	Server *tmp = my_client->getMyServer();
	std::vector<std::string> tmp_compTab = tmp->GetComptab();
	std::vector<void (Command::*)(std::string leftovers)> tmp_functionTab = tmp->GetFunctionTab();
	int j(0);
	for (std::vector<std::string>::iterator it = tmp_compTab.begin(); it != tmp_compTab.end(); it++)
	{
		if (*it == com)
		{
			// tmp_functionTab[j];
			// (this->*tmp_functionTab[j])(std::string leftovers);
			void (Command::*functionPointeur)(std::string) = tmp_functionTab[j];
			(this->*functionPointeur)(leftover);
		}
		// else
		// 	std::cerr << "NOP DOPE" << std::endl;
		// free-delete ici ?
		j++;
	}
}

void Command::capls(std::string leftovers)
{
	std::cout << "++++++++++++ je suis dans capls voici le leftorvers : " << leftovers << " +++++++++++++" << std::endl;
}

void Command::user(std::string leftovers)
{
	std::cout << "++++++++++++ je suis dans user voici le leftorvers : " << leftovers << " +++++++++++++" << std::endl;
}

void Command::nick(std::string leftovers)
{
	std::cout << "++++++++++++ je suis dans nick voici le leftorvers : " << leftovers << " +++++++++++++" << std::endl;
}

void Command::pass(std::string leftovers)
{
	std::cout << "++++++++++++ je suis dans pass voici le leftorvers : " << leftovers << " +++++++++++++" << std::endl;
}

void Command::ping(std::string leftovers)
{
	std::cout << "++++++++++++ je suis dans ping voici le leftorvers : " << leftovers << " +++++++++++++" << std::endl;
}












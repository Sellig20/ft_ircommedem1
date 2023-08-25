#include "Command.hpp"

Command::Command()
{

}

Command::~Command()
{

}

Command::Command(std::string com, std::string leftover, Client *my_client)
{
	// std::vector<
	(void)leftover;

	Server *tmp = my_client->getMyServer();
	// if (tmp == NU:)

    // std::cout << "AFTER ADDING THE SERVER = " << my_client->getMyServer()->GetServerName() << std::endl;
	// exit(1);
	std::vector<std::string> tmp_compTab = tmp->GetComptab();
	std::vector<Command::fct*> tmp_functionTab = tmp->GetFunctionTab();
	// std::cout << "Commande ----> " << com << " | Reste = " << leftover << std::endl;
		int j(0);
	for (std::vector<std::string>::iterator it = tmp_compTab.begin(); it != tmp_compTab.end(); it++)
	{
        // std::cout << "tab i = " << compTab[i] << " | level = " << com << std::endl;
		if (*it == com)
		{
			std::cout << "it => " << *it << std::endl;
			std::cout << "com => " << com << std::endl;
			std::cout << "j => " << j << std::endl;
			tmp_functionTab[j];
			(this->*(*it))();
		}
		j++;
	}
}

//  size_t index = std::distance(tmp_compTab.begin(), it);

//         // Vérifier que l'indice est valide
//         if (index < tmp_functionTab.size()) {
//             // Appeler la fonction pointée par l'indice
//             Server::fct func = tmp_functionTab[index];
//             (my_server->*func)(); // Appel de la fonction
//         } else {
//             std::cerr << "Indice en dehors de la plage du vecteur tmp_functionTab." << std::endl;
//         }

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

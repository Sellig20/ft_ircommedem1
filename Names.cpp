#include "../includes/Command.hpp"

void Command::names()
{
	std::cout << "++++++++++++ je suis dans names voici le leftorvers : " << command_leftovers << " +++++++++++++" << std::endl;
	// for(std::vector<Client*>::iterator it = ; it != )

	if (!command_leftovers.empty())
		std::cerr << "Option is not valid" << std::endl;
	else
	{
		//lister tous les noms des utilisateurs.
		// GET CONNECTEDDDDD
	}
}

#include "../includes/Command.hpp"

void Command::mode()
{
	// std::cout << "++++++++++++ je suis dans mode voici le leftovers : " << command_leftovers << std::endl;
	std::vector<Client *> gg = GetConnectedClient();
	// Client *my_c = getNickname();
	// std::cout << "?????????????????? " << my_c << std::endl;
	for (std::vector<Client *>::iterator it = gg.begin(); it != gg.end(); it++)
	{
		std::cout << "???????? " << *it << std::endl;
	}
	if (command_leftovers == "+i")
		std::cout << "INVISIBLE MODE ACTIVATED" << std::endl;
	else if (command_leftovers == "+t")
		std::cout << "RESTRICTION MODE ACTIVATED" << std::endl;
	else if (command_leftovers == "+k")
		std::cout << "KEY MODE ACTIVATED" << std::endl;
	else if (command_leftovers == "+o")
		std::cout << "OP MEMBER MODE ACTIVATED" << std::endl;
	else if (command_leftovers == "+l")
		std::cout << "LIMITED MODE ACTIVATED" << std::endl;

}

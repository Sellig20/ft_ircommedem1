#include "../includes/Command.hpp"

void Command::mode(std::string leftovers)
{
	std::cout << "++++++++++++ je suis dans mode voici le leftovers : " << leftovers << std::endl;
	std::vector<Client *> gg = GetConnectedClient();
	// Client *my_c = getNickname();
	// std::cout << "?????????????????? " << my_c << std::endl;
	for (std::vector<Client *>::iterator it = gg.begin(); it != gg.end(); it++)
	{
		std::cout << "???????? " << *it << std::endl;
	}
	if (leftovers == "+i")
		std::cout << "INVISIBLE MODE ACTIVATED" << std::endl;
	else if (leftovers == "+t")
		std::cout << "RESTRICTION MODE ACTIVATED" << std::endl;
	else if (leftovers == "+k")
		std::cout << "KEY MODE ACTIVATED" << std::endl;
	else if (leftovers == "+o")
		std::cout << "OP MEMBER MODE ACTIVATED" << std::endl;
	else if (leftovers == "+l")
		std::cout << "LIMITED MODE ACTIVATED" << std::endl;

}

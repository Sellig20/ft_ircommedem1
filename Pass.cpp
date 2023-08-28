#include "../includes/Command.hpp"
// #include "../includes/errors_rfc.hpp"

void Command::pass()
{
	if (my_client->getMyServer()->is_client_registered(my_client) == true)
	{
		error_code = "462";
		//a faire ici encore 
	}
	else
	{
		std::cout << "my server pass = [" << my_client->getMyServer()->GetServerPassword() << "this->comd_left = " << this->command_leftovers << std::endl;
		if (my_client->getMyServer()->GetServerPassword() == this->command_leftovers)
		{
			my_client->setIsRegistered(true);
			my_client->getMyServer()->add_to_registered_clients(my_client);
			response_buffer = "001 : ";
			response_buffer += "password accepted";
			is_ready = true;
			is_not_accepted = false;
			// std::cout << "password accepted client registered" << std::endl;
		}
		else
		{
			my_client->setRequestCode("464");
			response_buffer = "464 : ";
			response_buffer += "Password incorrect";
			is_ready = true;
			is_not_accepted = true;
		}
	}
}
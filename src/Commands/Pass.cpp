#include "../includes/Command.hpp"
// #include "../includes/errors_rfc.hpp"

void Command::pass()
{
	//PAS De NICL A CET INSTANT, PAS DE VERIFICIATION POSSIBLE
	// if (my_client == NULL)
	// {
	// 	std::cout << "ICICIICIC" << std::endl;
	// }
	if (my_client->getMyServer()->is_client_registered(my_client) == true)
	{
		error_code = "462";
		//a faire ici encore 
	}
	else
	{
		if (my_client->getMyServer()->GetServerPassword() == this->command_leftovers)
		{
			my_client->setIsRegistered(true);
			status = SINGLE_SEND;
			error_code = "001";
			response_buffer = "password accepted";
			is_ready = true;
			is_not_accepted = false;
			my_client->setMyPassword(true);
		}
		else
		{
			my_client->setIsRegistered(false);
			my_client->setRequestCode("464");
			status = SINGLE_SEND;
			// response_buffer = "464 : ";
			response_buffer = "Password incorrect";
			is_ready = true;
			is_not_accepted = true;
		}
	}
}
#include "../includes/Command.hpp"
// #include "../includes/errors_rfc.hpp"

void Command::pass()
{
	if (command_leftovers.empty())
		fill_error_need_more_params(this);
	else if (my_client->getIsRegistered() == true)
	{
		fill_error_already_registered();
	}
	else
	{
		if (my_client->getMyServer()->GetServerPassword() == this->command_leftovers)
		{
			my_client->setIsRegistered(true);
			my_client->setMyPassword(true);
		}
		else
		{
			fill_error_password_mismatch(this);
		}
	}
}
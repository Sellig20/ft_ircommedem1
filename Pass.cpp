#include "../includes/Command.hpp"
// #include "../includes/errors_rfc.hpp"

/* ERROR PASS 
	461 ok
	462 ok
	464 ok
*/

void Command::pass()
{
	if (command_leftovers.empty())
	{
		fill_error_need_more_params(this);
		// return ;
	}
	else if (my_client->getIsRegistered() == true)
	{
		fill_error_already_registered();
	}
	else
	{
		if (my_client->getMyServer()->GetServerPassword() != this->command_leftovers)
		{
			fill_error_password_mismatch(this);
		}
	}
}
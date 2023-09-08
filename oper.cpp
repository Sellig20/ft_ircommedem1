#include "../includes/Command.hpp"

void Command::oper()
{
	std::cout << "++++++++++++ je suis dans oper voici le leftorvers : " << command_leftovers << "+++++++++++++" << std::endl;

	std::string concerned;
	std::string password;



    std::istringstream iss(command_leftovers);
    iss >> concerned >> password;
	if (concerned.empty())
	{
		fill_error_need_more_params(this);
		return ;
	}
	if (my_client->getMyServer()->is_my_client_registered(concerned) == true)
	{
		if (password == "kikito" && my_client->GetIsOper() == false)
		{
			my_client->setIsOper(true);
			is_not_accepted = false;
			my_client->setRequestCode("381");
			error_code = "381";
			response_buffer = ":You are now an operator on this server";
			is_ready = true;
			if (!my_client->getNickname().empty())
				setConcernedClients(my_client->getNickname());
			setStatus(SINGLE_SEND);
		}
		else
		{
			fill_error_password_mismatch(this);
		}
	}
	else
	{
		is_not_accepted = true;
		my_client->setRequestCode("491");
		error_code = "491";
		response_buffer = ":No such user on server";
		is_ready = true;
		if (!my_client->getNickname().empty())
			setConcernedClients(my_client->getNickname());
		setStatus(SINGLE_SEND);

	}
}


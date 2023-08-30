#include "../includes/Command.hpp"


void Command::whois()
{
	// std::cout << "!!!!!!!!!WHO IS " << std::endl;
	response_buffer = "318 " + my_client->getNickname() + " " + my_client->getNickname() + " " + my_client->getUsername() + " " + my_client->getHostname() + " * :" + my_client->getRealname();
	
	// // = "311 YNNAF parallels * :parallels";
	is_not_accepted = false;
	my_client->setRequestCode("311");
	is_ready = true;
	// exit(1);
}
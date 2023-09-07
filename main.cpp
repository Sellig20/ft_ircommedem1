#include "includes/Server.hpp"
#include "includes/Client.hpp"
#include <iostream>
#include <cstdlib>
#include <csignal>

volatile sig_atomic_t receivedSIGINT = 0;
Server	*global_server = 0;

void handleSIGINT(int signum) {
    // receivedSIGINT = 1;÷
	(void)signum;
	if (global_server)
	{
		global_server->clean_them_all();
		// std::cout << "global cleaning" << std::endl;
	}
	delete global_server;
	// std::cout << "delete the server named = " << Server::getInstance().GetServerName() << std::endl;
	exit(1);
    // Ajoutez ici le code pour effectuer le nettoyage approprié
    // Par exemple, fermer les sockets, libérer la mémoire, etc.
}

int main (int argc, char **argv)
{
    if (argc != 3)
    {
        std::cerr << "Wrong input: usage: ./irc_server port_number password" << std::endl;
        return (-1);
    }

	struct sigaction sa;
    sa.sa_handler = handleSIGINT;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);

    Server *king_of_servers = new Server(atoi(argv[1]), argv[2]);

	if (argc == 2)
	{
		// Client *test_1(king_of_servers->GetEpollFD(), king_of_servers-)
		// king_of_servers->process_received_request()

		
		
	}
	// Client *my_bot = new Client(king_of_servers->GetServerSocketFD(), king_of_servers->GetEpollFD());
	// my_bot->setIsRegistered(true);
	// my_bot->setNickname("fat_bot");
	// my_bot->setUsername("fat_booty");
	// my_bot->setRealname("you most beautiful bot");
	// king_of_servers->setMyBot(my_bot);
	// my_bot->run_the_bot();
	
	// my_bot->redaction_answer_request()

	// Server::setInstance(king_of_servers);
	// std::cout << "naming the server named = " <<  Server::getInstance().GetServerName() << std::endl;

	global_server = king_of_servers;

	while (!receivedSIGINT) {
    	// my_bot->run_the_bot();
		king_of_servers->loop_running_server();
		
	}



}

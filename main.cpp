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
        std::cerr << "Wrong input: usage: ./irc port_number" << std::endl;
        return (-1);
    }

	struct sigaction sa;
    sa.sa_handler = handleSIGINT;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);

    Server *king_of_servers = new Server(atoi(argv[1]), argv[2]);

	// Server::setInstance(king_of_servers);
	std::cout << "naming the server named = " <<  Server::getInstance().GetServerName() << std::endl;

	global_server = king_of_servers;

	while (!receivedSIGINT) {
    	king_of_servers->loop_running_server();
	}



}

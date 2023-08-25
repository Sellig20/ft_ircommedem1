#include "includes/Server.hpp"
#include "includes/Client.hpp"
#include <iostream>
#include <cstdlib>
#include <csignal>

volatile sig_atomic_t receivedSIGINT = 0;

void handleSIGINT(int signum) {
    receivedSIGINT = 1;
	(void)signum;
    std::cout << "Received SIGINT. Cleaning up..." << std::endl;
	Server::getInstance().clean_them_all();
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

    Server king_of_servers(atoi(argv[1]), argv[2]);

	while (!receivedSIGINT) {
    	king_of_servers.loop_running_server();
	}



}

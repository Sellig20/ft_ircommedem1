#include "includes/Server.hpp"
#include "includes/Client.hpp"
#include <iostream>
#include <cstdlib>
#include <csignal>

static Server	*g_server = 0;

void handleSIGINT(int signum) {
	(void)signum;
	if (g_server)
	{
		g_server->clean_them_all();
	}
	delete g_server;
	exit(1);
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
	g_server = king_of_servers;

	while (1)
	{
		king_of_servers->loop_running_server();
	}


}

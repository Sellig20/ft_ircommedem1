#include "includes/Server.hpp"
#include "includes/Client.hpp"

int main (int argc, char **argv)
{
    if (argc != 2)
    {
        std::cerr << "Wrong input: usage: ./irc port_number" << std::endl;
        return (-1);
    }
    Server king_of_servers(atoi(argv[1]));
    king_of_servers.loop_running_server();



}

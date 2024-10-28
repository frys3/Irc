#include "inc/server.hpp"
#include "inc/irc.hpp"

int valid_args(std::string port, std::string mdp)
{
    if (port.size() < 1 || port.size() > 5)
        return (0);
    for (size_t i = 0; i < port.size(); i++)
    {
        if (!isdigit(port[i]))
            return (0);
    }
    if (mdp.size() < 1 || atoi(port.c_str()) == 0)
        return (0);
    return (1);
}

int main(int ac, char **av)
{
    try
    {
    	if (ac != 3 || !valid_args(av[1], av[2]))
			throw invalid_args();
        Server serv(av[1], av[2]);
		serv.loop();
    }
    catch (std::exception & e)
    {
		std::cout << e.what() << std::endl;
        return (1);
    }
    return (0);
}

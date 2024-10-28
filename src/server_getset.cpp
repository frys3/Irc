#include "../inc/server.hpp"

void			Server::set_serv_socket(int sock)
{
    _server_socket = sock;
}

int				Server::get_serv_socket(void)
{
    return (_server_socket);
}

struct pollfd 	*Server::get_pollfd(void)
{
    return (_poll_fds);
}

int				Server::get_poll_count(void)
{
    return (_poll_count);
}

int 			Server::get_port(void)
{
    return (_port);
}

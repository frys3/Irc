#include "../inc/server.hpp"

bool	Server::quit(User & client, std::string cmd)
{
	(void)cmd;
	del_user(client.getFd());
	return true;
}

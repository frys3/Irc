#include "../inc/server.hpp"

bool	Server::capLs(User & client, std::string cmd)
{
	if (cmd != "LS")
		return 0;
	if (client.getStatus() == 0) 
		client.setStatus(1);
	return 0;	
}

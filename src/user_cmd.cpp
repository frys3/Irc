#include "../inc/server.hpp"

bool Server::user(User & client, std::string cmd)
{
	std::string					msg;
	std::string					name = cmd;
    std::string::size_type		end;

	if ((end = cmd.find(" ", 0)) != std::string::npos)
		name = cmd.substr(0, end);
 	client.setUsername(name);
	if (client.getStatus() == 3)
	{
		client.setStatus(4);
 		msg = ":localhost 001 " + client.getNick() + " :Welcome to bdtServer " + client.getNick() + "!~" + client.getUsername() + "@127.0.0.1\r\n";

 		if (send(client.getFd(), msg.c_str(), msg.size(), MSG_NOSIGNAL) < 1)
		{
			del_user(client.getFd());
			return (false);
		}
		return (true);
	}
	return (false);
}

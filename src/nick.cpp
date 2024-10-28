#include "../inc/server.hpp"

bool	Server::is_valid_nick(std::string nick, std::string &msg)
{
	if (nick.size() < 1)
	{
    	msg = ":" + nick + "!~" + "*@bdtServer 431 " + ": No nickname given\r\n";
		return (false);
	}
	for (std::map<int, User>::iterator it = _users.begin(); it != _users.end(); ++it)
	{
		if (it->second.getNick() == nick)
		{
    		msg = ":localhost 433 * " + nick + " :Nickname is already in use.\r\n";
			return (false);
		}
	}
	for (size_t i = 0; i < nick.size(); ++i)
	{
		if (!std::isalnum(nick[i]))
		{
    		msg = ":" + nick + "!~" + "*@bdtServer 432 " + ": Erroneous nickname\r\n";
			return (false);
		}
	}
	return (true);
}

bool Server::nick(User & client, std::string cmd)
{
	std::string					msg("");
    std::string::size_type		space;
	std::string					nick_msg("");

	if (client.getStatus() != 2 && client.getStatus() != 4)
		return (false);
	if ((space = cmd.find(" ", 0)) != std::string::npos)
		cmd = cmd.substr(0, space);
	if (!is_valid_nick(cmd, msg))
	{
    	if (send(client.getFd(), msg.c_str(), msg.size(), MSG_NOSIGNAL) < 1)
			del_user(client.getFd());
	   	return (false);
	}
	msg = ":" + client.getNick() + " NICK :" + cmd + "\r\n";
	if (client.getStatus() == 4)
	{
		for (std::map<int, User>::iterator it = _users.begin(); it != _users.end(); it ++)
		{
			if (send(it->second.getFd(), msg.c_str(), msg.size(), MSG_NOSIGNAL) < 1)
			{
				del_user(it->first);
				return (false);
			}
		}
	}
	client.setNick(cmd);
	if (client.getStatus() == 2)
		client.setStatus(3);
	if (client.getUsername() != "" && client.getStatus() != 4)
	{
		msg = ":localhost 001 " + client.getNick() + " :Welcome to bdtServer " + client.getNick() + "!~" + client.getUsername() + "@127.0.0.1\r\n";
 		if (send(client.getFd(), msg.c_str(), msg.size(), MSG_NOSIGNAL) < 1)
		{
			del_user(client.getFd());
			return (false);
		}
		client.setStatus(4);
	}
	return (true);
}

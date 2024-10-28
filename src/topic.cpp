#include "../inc/server.hpp"
#include "../inc/irc.hpp"


bool	Server::topic(User &client, std::string cmd)
{
	std::string serv_msg;
	std::string chan = extract(cmd, "#", ":");
	std::string newtopic = extract(cmd, ":", "\0");
	if (_channels[chan].getMTopic() == true && !_channels[chan].isOperator(client.getFd()))
	{
		serv_msg = ":localhost 482 #" + chan + " :You're not channel operator\r\n";
		if (send(client.getFd(), serv_msg.c_str(), serv_msg.size(), MSG_NOSIGNAL) < 1)
            return (false);
		return (false);
	}
	serv_msg = ":" + client.getNick() + "!" + client.getUsername() + "@localhost TOPIC #" + chan + " :" + newtopic + "\r\n";
	for (std::vector<int>::iterator it = _channels[chan].getUsers().begin(); it != _channels[chan].getUsers().end(); it++)
	{
		if (send(*it, serv_msg.c_str(), serv_msg.size(), MSG_NOSIGNAL) < 1)
            return (false);
	}
	for (std::vector<int>::iterator it = _channels[chan].getOperators().begin(); it != _channels[chan].getOperators().end(); it++)
	{
		if (send(*it, serv_msg.c_str(), serv_msg.size(), MSG_NOSIGNAL) < 1)
            return (false);
	}
	_channels[chan].setTopic(newtopic);
	return (1);
}


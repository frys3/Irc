#include "../inc/server.hpp"

bool	Server::part(User & client, std::string cmd)
{
	std::string		msg;
	std::string		channel = cmd.erase(0, 1);
	std::map<std::string, Channel>::iterator it_channel = _channels.find(channel);

	if (it_channel == _channels.end())
	{
		msg = ":localhost 403 " + client.getNick() + " " + channel + " :No such channel\r\n";
		if (send(client.getFd(), msg.c_str(), msg.size(), MSG_NOSIGNAL) < 1)
			del_user(client.getFd());
		return false;
	}
	std::vector<int>::iterator it_users;
	std::vector<int>::iterator ite_users = it_channel->second.getUsers().end();
	std::vector<int>::iterator it_operators;
	std::vector<int>::iterator ite_operators = it_channel->second.getOperators().end();
	for (it_users = it_channel->second.getUsers().begin(); it_users != ite_users ; it_users++)
	{
		if ((*it_users) == client.getFd())
			break;
	}
	for (it_operators = it_channel->second.getOperators().begin() ; it_operators != ite_operators ; it_operators++)
	{
		if ((*it_operators) == client.getFd())
			break;
	}
	if (it_users == ite_users && it_operators == ite_operators)
	{
		msg = ":localhost 442 " + client.getNick() + " " + channel + " :You're not on that channel\r\n";
		if (send(client.getFd(), msg.c_str(), msg.size(), MSG_NOSIGNAL) < 1)
			del_user(client.getFd());
		return false;
	}
	if (it_users != ite_users)
		it_channel->second.getUsers().erase(it_users);
	else
		it_channel->second.getOperators().erase(it_operators);
	msg = ":" + client.getNick() + "@localhost PART #" + channel + "\r\n";
	if (send(client.getFd(), msg.c_str(), msg.size(), MSG_NOSIGNAL) < 1)
	{
		del_user(client.getFd());
		return false;
	}
	return true;
}

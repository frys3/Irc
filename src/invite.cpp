#include "../inc/server.hpp"

bool	Server::initNickAndChan(std::string &nick, std::string &channel, User &client, std::string cmd)
{
	std::string					msg;

	if (cmd.find(" ", 0) != std::string::npos)
		nick = cmd.substr(0, cmd.find(" ", 0));
	cmd.erase(0, cmd.find(" ", 0) + 1);
	channel = cmd;
	if (channel[0] != '#')
	{
		if (nick[0] != '#')
		{
			msg = ":localhost 401 " + client.getNick() + " " + channel + " :No such nick/channel\r\n";
			if (send(client.getFd(), msg.c_str(), msg.size(), MSG_NOSIGNAL) < 1)
			{
				del_user(client.getFd());
				return false;
			}
			return false;
		}
		else
			std::swap(nick, channel);
	}
	channel = channel.erase(0, 1);
	return true;
}

bool	Server::checkGuest(std::map<int, User>::iterator &it_guest, User & client, std::string &nick, std::string &channel)
{
	std::string					msg;

	for (it_guest = _users.begin() ; it_guest != _users.end() ; it_guest++)
	{
		if (it_guest->second.getNick() == nick)
			break;
	}
	if (it_guest == _users.end())
	{
		msg = ":localhost 401 " + client.getNick() + " " + channel + " :No such nick/channel\r\n";
		if (send(client.getFd(), msg.c_str(), msg.size(), MSG_NOSIGNAL) < 1)
		{
			del_user(client.getFd());
			return false;
		}
		return false;
	}
	return true;
}

void	Server::addToWhitelist(std::map<std::string, Channel>::iterator &it_channel,
			std::map<int, User>::iterator &it_guest, User & client, std::string channel)
{
	std::string	msg;

	it_channel->second.getWhitelist().push_back(it_guest->second.getFd());
	msg = ":localhost 341 " + client.getNick() + " " + it_guest->second.getNick() + " #" + channel + "\r\n";
	if (send(client.getFd(), msg.c_str(), msg.size(), MSG_NOSIGNAL) < 1)
	{
		del_user(client.getFd());
		return ;
	}
	msg = ":localhost NOTICE @#" + channel + " :" + client.getNick() + " invited " + it_guest->second.getNick() + " into channel " + " #" + channel + "\r\n";
	if (send(client.getFd(), msg.c_str(), msg.size(), MSG_NOSIGNAL) < 1)
	{
		del_user(client.getFd());
		return ;
	}
	msg = ":" + client.getNick() + "!~" + client.getUsername() + "@localhost INVITE " + it_guest->second.getNick() + " :#" + channel + "\r\n";
	if (send(it_guest->second.getFd(), msg.c_str(), msg.size(), MSG_NOSIGNAL) < 1)
	{
		del_user(it_guest->second.getFd());
		return ;
	}
}

bool	Server::invite(User & client, std::string cmd)
{
	std::string					nick("");
	std::string					channel("");
	std::string					msg;

	if(!initNickAndChan(nick, channel, client, cmd))
		return false;

	std::map<std::string, Channel>::iterator it_channel = _channels.find(channel);
	if (it_channel != _channels.end())
	{
		std::map<int, User>::iterator it_guest;
		if (!checkGuest(it_guest, client, nick, channel))
			return false;
		std::vector<int>::iterator it_users;
		std::vector<int>::iterator it_operators;
		for (it_users = it_channel->second.getUsers().begin() ; it_users != it_channel->second.getUsers().end() ; it_users++)
		{
			if ((*it_users) == it_guest->second.getFd())
				break;
		}
		for (it_operators = it_channel->second.getOperators().begin() ; it_operators != it_channel->second.getOperators().end() ; it_operators++)
		{
			if ((*it_operators) == it_guest->second.getFd())
				break;
		}
		if (it_users == it_channel->second.getUsers().end() && it_operators == it_channel->second.getOperators().end())
		{
			std::vector<int>::iterator it_oper;
			for (it_oper = it_channel->second.getOperators().begin() ; it_oper != it_channel->second.getOperators().end() ; it_oper++)
				if ((*it_oper) == client.getFd())
					break;
			if (it_oper == it_channel->second.getOperators().end())
			{
				msg = ":localhost 482 " + client.getNick() + " #" + channel + " :You're not channel operator\r\n";
				if (send(client.getFd(), msg.c_str(), msg.size(), MSG_NOSIGNAL) < 1)
					del_user(client.getFd());
				return false;
			}
			else
			{
				addToWhitelist(it_channel, it_guest, client, channel);
				return false;
			}
		}
		else
		{
			msg = ":localhost 443 " + client.getNick() + " " + it_guest->second.getNick() + " :#" + channel + " :is already on channel\r\n";
			if (send(client.getFd(), msg.c_str(), msg.size(), MSG_NOSIGNAL) < 1)
				del_user(client.getFd());
			return false;
		}
	}
	else
	{
		msg = ":localhost 403 " + client.getNick() + " " + channel + " :No such channel\r\n";
		if (send(client.getFd(), msg.c_str(), msg.size(), MSG_NOSIGNAL) < 1)
			del_user(client.getFd());
		return false;
	}
	return true;
}

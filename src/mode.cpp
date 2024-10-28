#include "../inc/server.hpp"

bool	Server::mode_send_message(int fd, std::string msg)
{
	if (send(fd, msg.c_str(), msg.size(), MSG_NOSIGNAL) < 1)
	{
		del_user(fd);
		return (false);
	}
	return (true);
}

bool	Server::mode_check_chan(std::vector<std::string> & param, User & client)
{
	std::string					msg;

	if (param[0] == client.getNick())
		return (false);
	if (param[0].size() < 2 || param[0][0] != '#' || _channels.find(param[0].substr(1, param[0].size())) == _channels.end())
	{
		mode_send_message(client.getFd(), ":localhost 403 " + client.getNick() + " " + param[0] + " :No such channel\r\n");
		return (false);
	}
	param[0].erase(0, 1);
	return (true);
}

bool	Server::mode_print_info(User & client, std::string name, Channel &chan)
{
	std::string					msg;

	msg = ":localhost 324 " + client.getNick() + " #" + name + " +";
	if (chan.getMTopic() == true)
		msg += "t";
	if (chan.getInvit() == true)
		msg += "i";
	if (chan.getMaxUser() != -1)
		msg += "l";
	if (chan.getPassword() != "")
		msg += "k";
	if (chan.getMaxUser() != -1)
	{
		std::stringstream ss;
    	ss << chan.getMaxUser();
    	std::string str = ss.str();
		msg += " " + str;
	}
	if (chan.getPassword() != "")
		msg += " " + chan.getPassword();
	msg += "\r\n";
	if (mode_send_message(client.getFd(), msg) == false)
		return (false);
	return (true);
}

bool	Server::mode_check_option(std::string option, User & client, std::string chan)
{
	int				j = 0;
	int				i = 0;
	int				o = 0;
	int				t = 0;
	int				l = 0;
	int				k = 0;

	while (j < static_cast<int>(option.size()))
	{
		if (option[j] == 'i')
			i++;
		else if (option[j] == 'o')
			o++;
		else if (option[j] == 't')
			t++;
		else if (option[j] == 'l')
			l++;
		else if (option[j] == 'k')
			k++;
		j++;
	}
	if (i > 1 || o > 1 || t > 1 || l > 1 || k > 1)
	{
		mode_send_message(client.getFd(), ":localhost " + client.getNick() + " " + chan + " :Syntax error in the option\r\n");
		return (false);
	}
	return (true);
}

void	Server::mode_set_message(std::string & msg, std::string & res, char c, bool type, std::vector<std::string> &param)
{
	(void)res;
	(void)c;
	(void)param;
	if (*msg.rbegin() == ' ')
	{
		if (type == true)
			msg += "+";
		else
			msg += "-";
	}
	else
	{
		if (type == true)
		{
			if (msg.rfind("-") != std::string::npos)
			{
				if (msg.rfind("+") == std::string::npos || (msg.rfind("+") != std::string::npos && msg.rfind("+") < msg.rfind("-")))
					msg += "+";
			}
		}
		else
		{
			if (msg.rfind("+") != std::string::npos)
			{
				if (msg.rfind("-") == std::string::npos || (msg.rfind("-") != std::string::npos && msg.rfind("-") < msg.rfind("+")))
					msg += "-";
			}
		}
	}
	if (c == 'o' || c == 'k' || (c == 'l' && type == true))
	{
		res += " " + param[2];
		param.erase(param.begin() + 2);
	}
	msg += c;
}

bool	Server::mode(User &client, std::string cmd)
{
	std::vector<std::string>			param;		
	Channel								*chan;
	std::string							option("itklo");		
	std::string							msg;
	std::string							res;		
    std::string::size_type				end;
	std::vector<std::string>::iterator	it;
	std::map<int, User>::iterator 		iter = _users.begin();
	bool								type = true;
	
	while ((end = cmd.find(" ", 0)) != std::string::npos)
	{
		param.push_back(cmd.substr(0, end));
		cmd.erase(0, end + 1);
	}
	param.push_back(cmd);
	msg = ":" + client.getUsername() + "!~" + client.getNick() + "@127.0.0.1.ip MODE " + param[0] + " ";
	if (mode_check_chan(param, client) == false)
		return (false);	
	else
		chan = &_channels[param[0]];
	if (param.size() == 1)
		return (mode_print_info(client, param[0], *chan));
	if (std::find(chan->getOperators().begin(), chan->getOperators().end(), client.getFd()) == chan->getOperators().end())
	{
		mode_send_message(client.getFd(), ":localhost 482 " + client.getNick() + " #" + param[0] + " :You're not channel operator\r\n");
		return (false);
	}
	if (mode_check_option(param[1], client, param[0]) == false)
		return (false);
	for (std::string::iterator ite = param[1].begin(); ite != param[1].end() ; ite++)
	{
		if (*ite == '+')
			type = true;
		else if (*ite == '-')
			type = false;
		else if ((end = option.find(*ite)) != std::string::npos)
		{
			switch (end)
			{
				case 0:
					if (type == true)
						chan->setInvit(true);
					else
						chan->setInvit(false);
					mode_set_message(msg, res, 'i', type, param);
					break;
				case 1:
					if (type == true)
						chan->setMTopic(true);
					else
						chan->setMTopic(false);
					mode_set_message(msg, res, 't', type, param);
					break;
				case 2:
					if (type == true)
					{
						if (param.size() < 3)
							break;
						chan->setPassword(param[2]);
					}
					else
						chan->setPassword("");
					mode_set_message(msg, res, 'k', type, param);
					break;
				case 3:
					if (type == true)
					{
						if (param.size() < 3)
						{
							mode_send_message(client.getFd(), ":localhost 461 " + client.getNick() + " MODE +l :Not enough parameters\r\n");
							break;
						}
						std::istringstream iss(param[2].c_str());
						int result;
						if ((iss >> result))
							chan->setMaxUser(result);
					}
					else
						chan->setMaxUser(-1);
					mode_set_message(msg, res, 'l', type, param);
					break;
				case 4:
					if (param.size() < 3)
						break;
					while (iter != _users.end())
					{
						if (iter->second.getNick() == param[2])
						{
							if (chan->isInChan(iter->second.getFd()) == false)
							{
								if (mode_send_message(client.getFd(), ":localost 441 " + client.getNick() + " " + param[2] + " :They aren't on that channel\r\n") == false)
									return (false);
							}
							break;
						}
						iter++;
					}
					if (iter == _users.end() || chan->isInChan(iter->second.getFd()) == false)
						break;
					if (type == true)
					{
						if (chan->isOperator(iter->second.getFd()) == false)
						{
							chan->addOperator(iter->second.getFd());
							chan->delUser(iter->second.getFd());
						}
					}
					else
					{
						if (chan->isOperator(iter->second.getFd()) == true)
						{
							chan->addUser(iter->second.getFd());
							chan->delOperator(iter->second.getFd());
						}
					}
					mode_set_message(msg, res, 'o', type, param);
					break;
					
			}
		}
		else
		{
			if (!mode_send_message(client.getFd(), ":localhost 472 " + client.getNick() + " " + *ite + " :is unknown mode char to me\r\n"))
				return (false);
		}
	}
	if (*(msg.end() - 1) == ' ')
		return (true);
	msg += res + "\r\n";
    for (std::vector<int>::iterator ite = chan->getUsers().begin(); ite != chan->getUsers().end(); ite++)
			mode_send_message(*ite, msg);
    for (std::vector<int>::iterator ite = chan->getOperators().begin(); ite != chan->getOperators().end(); ite++)
			mode_send_message(*ite, msg);
    return (true);
}

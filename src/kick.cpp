#include "../inc/server.hpp"

void Server::kick_from_a_chan(std::string &chan, int fd_to_kick)
{
    std::vector<int>::iterator it = std::find(_channels[chan].getUsers().begin(), _channels[chan].getUsers().end(), fd_to_kick);
    if (it != _channels[chan].getUsers().end())
        _channels[chan].getUsers().erase(it);

    it = std::find(_channels[chan].getOperators().begin(), _channels[chan].getOperators().end(), fd_to_kick);
        if (it != _channels[chan].getOperators().end())
        _channels[chan].getOperators().erase(it);
    
    std::vector<std::string>::iterator ite = std::find(_users[fd_to_kick].getChannels().begin(),_users[fd_to_kick].getChannels().end(), chan);
    if (ite != _users[fd_to_kick].getChannels().end())
        _users[fd_to_kick].getChannels().erase(ite);
}

bool Server::kick(User &client, std::string cmd)
{
    std::string serv_msg;
    std::string chan = cmd.substr(1, cmd.find(" ", 1) - 1);
    std::string user_to_kick = cmd.substr(2 + chan.size(), std::string::npos);
    user_to_kick.erase(user_to_kick.size() - 2, 2);

    int fd_to_kick = 0;
    for (std::map<int, User>::iterator it = _users.begin(); it != _users.end(); it++)
    {
        if (it->second.getNick() == user_to_kick)
        {
            fd_to_kick = it->second.getFd();
            break ;
        }
    } 
    if (fd_to_kick == 0)
    {
        serv_msg = ":localhost 441 " + client.getNick() + " " + user_to_kick + " #" + chan + " :They aren't on that channel\r\n";
        if (send(client.getFd(), serv_msg.c_str(), serv_msg.size(), MSG_NOSIGNAL) < 1)
                return (false);
        return (1);
    }
    
    if (!_channels[chan].isOperator(client.getFd()))
	{
		serv_msg = ":localhost 482 #" + chan + " :You're not channel operator\r\n";
		if (send(client.getFd(), serv_msg.c_str(), strlen(serv_msg.c_str()), MSG_NOSIGNAL) < 1)
            return (false);
        return (1);
	}
    
    serv_msg = ":" + client.getNick() + "!" + client.getUsername() + "@localhost KICK #" + chan + " " + user_to_kick + " :\r\n";
    for (std::vector<int>::iterator ite = _channels[chan].getUsers().begin(); ite != _channels[chan].getUsers().end(); ite++)
    {
            if (send(*ite, serv_msg.c_str(), serv_msg.size(), MSG_NOSIGNAL) < 1)
                return (false);
    }
    for (std::vector<int>::iterator ite = _channels[chan].getOperators().begin(); ite != _channels[chan].getOperators().end(); ite++)
    {
            if (send(*ite, serv_msg.c_str(), serv_msg.size(), MSG_NOSIGNAL) < 1)
                return (false);
    }
    kick_from_a_chan(chan, fd_to_kick);
    return (1);
}


#include "../inc/server.hpp"
#include "../inc/irc.hpp"

bool	Server::privmsg_user(User &client, std::string cmd)
{
    std::string serv_msg;
    std::string receiver = cmd.substr(0, cmd.find(" "));
    std::string msg_str = extract(cmd, ":", "\0");
    int fd_receiver = 0;
    for (std::map<int, User>::iterator it = _users.begin(); it != _users.end(); it++)
    {
        if (it->second.getNick() == receiver)
        {
            fd_receiver = it->second.getFd();
            break ;
        }
    }
    if (fd_receiver == 0)
    {
        serv_msg = ":localhost 401 " + receiver + " :No such nick/channel\r\n";
        if (send(client.getFd(), serv_msg.c_str(), serv_msg.size(), MSG_NOSIGNAL) < 1)
                return (false);
        return (1);
    }
    serv_msg = ":" + client.getNick() + "!localhost PRIVMSG " + receiver + " :" + msg_str + "\r\n";
    if (send(fd_receiver, serv_msg.c_str(), serv_msg.size(), MSG_NOSIGNAL) < 1)
                return (false);
    return (1);
}

bool	Server::privmsg(User &client, std::string cmd)
{
    if (cmd[0] != '#')
        return (privmsg_user(client, cmd));
    std::string msg_str = extract(cmd, ":", "\0");
    std::string channel_name = extract(cmd, "#", ":");
    std::string msg_to_send = ":" + _users[client.getFd()].getNick() + "!~" + _users[client.getFd()].getUsername() + "@localhost PRIVMSG #" + channel_name + " :" + msg_str + "\r\n";
    for (std::vector<int>::iterator ite = _channels[channel_name].getUsers().begin(); ite != _channels[channel_name].getUsers().end(); ite++)
    {
        if (*ite != client.getFd())
        {
            if (send(*ite, msg_to_send.c_str(), msg_to_send.size(), MSG_NOSIGNAL) < 1)
            return (false);
        }
    }
    for (std::vector<int>::iterator ite = _channels[channel_name].getOperators().begin(); ite != _channels[channel_name].getOperators().end(); ite++)
    {
        if (*ite != client.getFd())
        {
            if (send(*ite, msg_to_send.c_str(), msg_to_send.size(), MSG_NOSIGNAL) < 1)
            return (false);
        }
    }
    return (1);
}

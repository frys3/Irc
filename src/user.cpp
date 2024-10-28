#include "../inc/irc.hpp"

User::User() : _username("")
{
}

User::User(int fd) : _username(""), _fd(fd) , _status(0)
{
}

User::~User(void)
{
}

int					User::getFd(void) const
{
	return this->_fd;
}

int					User::getStatus(void) const
{
	return this->_status;
}

std::string		User::getBuffer(void) const
{
	return _buffer;
}

std::string		User::getNick(void) const
{
	return _nick;
}

std::string		User::getUsername(void) const
{
	return _username;
}

std::vector<std::string>	&User::getChannels(void)
{
	return (_channels);
}

void				User::setStatus(int status)
{
	_status = status;
}

void			User::setBuffer(std::string buffer)
{
	_buffer = buffer;
}

void			User::setNick(std::string nick)
{
	_nick = nick;
}

void			User::setUsername(std::string username)
{
	_username = username;
}

void			User::addChannel(std::string channel)
{
	_channels.push_back(channel);
}

void	User::delChannel(std::string channel)
{
	std::vector<std::string>::iterator pos = std::find(_channels.begin(), _channels.end(), channel);
	if (pos != _channels.end())
		_channels.erase(pos);
}

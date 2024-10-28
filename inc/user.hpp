#pragma once

#include <iostream>
#include <ostream>
#include <cstring>
#include <map>
#include <vector>
#include <algorithm>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

class	Channel;

class	User
{


	public:

		User(void);
		User(int fd);
		~User(void);

		std::string					getBuffer(void) const;
		std::string					getNick(void) const;
		std::string					getUsername(void) const;
		std::vector<std::string>	&getChannels(void);
		int							getFd(void) const;
		int							getStatus(void) const;
		void						setStatus(int status);
		void						setBuffer(std::string buffer);
		void						setNick(std::string nick);
		void						setUsername(std::string username);
		void						addChannel(std::string channel);
		void						delChannel(std::string channel);

	private:

    	std::string 				_nick;
    	std::string 				_username;
    	std::string 				_buffer;
    	int 						_fd;
    	int 						_status;
		std::vector<std::string>	_channels;
};

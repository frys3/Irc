
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
#include "user.hpp"

class	User;

class	Channel
{


	public:

		Channel(void);
		Channel(std::string name);
		~Channel(void);

		bool				getInvit(void) const;
		bool				getMTopic(void) const;
		int					getMaxUser(void) const;
		std::string			getPassword(void) const;
		std::string			getName(void) const;
		std::string			getTopic(void) const;
		std::vector<int>	&getUsers(void);
		std::vector<int>	&getOperators(void);
		std::vector<int>	&getWhitelist(void);
		void				setTopic(std::string topic);
		void				setMTopic(bool status);
		void				setInvit(bool status);
		void				setMaxUser(int limit);
		void				setPassword(std::string password);
		bool				isInChan(int fd) const;
		bool				isOperator(int fd) const;
		void				addUser(int user);
		void				delUser(int user);
		void				addOperator(int op);
		void				delOperator(int op);

	private:

    	std::string 				_name;
    	std::string 				_topic;
    	std::vector<int> 			_users;
    	std::vector<int>			_operators;
		std::vector<int>			_whitelist;
		bool						_m_invit;
		bool						_m_topic;
		int							_m_maxUser;
		std::string					_m_password;
};

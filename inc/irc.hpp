#pragma once

#include <errno.h>
#include <netdb.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/poll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <csignal>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <cstdlib>
#include <algorithm>
#include <cctype>
#include "user.hpp"
#include "channel.hpp"


#define PORT 6667

void accept_new_connection(int server_socket, struct pollfd **poll_fds, int *poll_count, int *poll_size);
void read_data_from_socket(int i, struct pollfd **poll_fds, int *poll_count, int server_socket);
void add_to_poll_fds(struct pollfd *poll_fds[], int new_fd, int *poll_count, int *poll_size);
void del_from_poll_fds(struct pollfd **poll_fds, int i, int *poll_count);
int	 valid_args(std::string port, std::string mdp);
std::string extract(const std::string& chaine, std::string begin, std::string end);

class	invalid_args :  public std::exception
{
	public:

		virtual const char * what() const throw();

};
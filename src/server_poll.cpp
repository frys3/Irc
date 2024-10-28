#include "../inc/server.hpp"

int Server::calloc_pollfd(int size)
{
    if (!this->_poll_fds)
    {
        this->_poll_fds = (struct pollfd *) calloc(size + 1, sizeof *this->_poll_fds);
        if (!this->_poll_fds)
            return (0);
    }
    return (1);
}

void Server::add_to_poll_fds(int new_fd)
{
    if (this->_poll_count == this->_poll_size) 
    {
        this->_poll_size *= 2;
        this->_poll_fds = (struct pollfd *) realloc(this->_poll_fds, sizeof(*(this->_poll_fds)) * (this->_poll_size));
        if (!this->_poll_fds)
            throw fatal_error();
    }
    this->_poll_fds[this->_poll_count].fd = new_fd;
    this->_poll_fds[this->_poll_count].events = POLLIN;
    this->_poll_count++;
}

void Server::del_from_poll_fds(int i)
{
    for (int j = 0; j < _poll_count; j++)
	{
        if (_poll_fds[j].fd == i)
        {
            this->_poll_fds[j] = this->_poll_fds[this->_poll_count - 1];
            this->_poll_count -= 1;
			return ;
        }
    }
}

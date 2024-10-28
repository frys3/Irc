#include "../inc/server.hpp"
#include "../inc/user.hpp"

Server* Server::_ptrServer = NULL;

Server::Server(char *port_str, char *mdp)
{
	_ptrServer = this;
    this->_poll_fds = NULL;
    this->_poll_size = 10;
    this->_poll_count = 1;

    this->_mdp = mdp;
    this->_port = atoi(port_str);
    _server_socket = create_server_socket(this->get_port());
    if (listen(this->get_serv_socket(), 20) == -1
    	|| !this->calloc_pollfd(this->_poll_size))
	{
		closeServer(0);
        throw init_failed();
	}
    this->_poll_fds[0].fd = this->_server_socket;
    this->_poll_fds[0].events = POLLIN;
    std::signal(SIGINT, closeServer);
    std::signal(SIGQUIT, closeServer);
	initCommand();
}

Server::~Server(void)
{
    if (this->_poll_fds)
    {
        int i = 0;
        while (i < _poll_count)
        {
            close(this->_poll_fds[i].fd);
            i++;
        }
        free(this->_poll_fds);
    }
}

void		Server::closeServer(int signal)
{
	close(_ptrServer->_server_socket);
	if (signal != 0)
		throw closing_server();
}

int Server::create_server_socket(int port) 
{
    struct sockaddr_in sa;
    int socket_fd;

    memset(&sa, 0, sizeof sa);
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = htonl(INADDR_ANY);
    sa.sin_port = htons(port);

    socket_fd = socket(sa.sin_family, SOCK_STREAM, 0);
    if (socket_fd == -1)
		throw init_failed();
	if (bind(socket_fd, (struct sockaddr *)&sa, sizeof sa) != 0)
	{
		close(socket_fd);
		throw init_failed();
	}
    return (socket_fd);
}

void      		Server::initCommand(void)
{
	_command["CAP"]     = &Server::capLs;
	_command["NICK"]    = &Server::nick;
	_command["USER"]    = &Server::user;
	_command["PASS"]    = &Server::pass;
	_command["PING"]    = &Server::ping;
	_command["QUIT"]    = &Server::quit;
	_command["JOIN"]    = &Server::join;
	_command["PRIVMSG"] = &Server::privmsg;
	_command["KICK"]    = &Server::kick;
	_command["TOPIC"]   = &Server::topic;
	_command["MODE"]    = &Server::mode;
	_command["INVITE"]  = &Server::invite;
	_command["PART"]    = &Server::part;
}

std::string 	Server::extract(const std::string& chaine, std::string begin, std::string end) 
{
    std::size_t debut = chaine.find(begin);
    std::size_t fin = chaine.find(end, debut + 1) - 1;
    if (debut != std::string::npos && fin != std::string::npos) 
        return chaine.substr(debut + 1, fin - debut - 1);
    else 
        return "";
}

void	Server::loop(void)
{
	std::cout << "---- SERVER ----\n" << std::endl;
	while (1) 
	{
	    int status = poll(_poll_fds,_poll_count, 9000);
	    if (status == -1)
	        return;
	    else if (status == 0)
	        continue;
	    for (int i = 0; i < _poll_count; i++)
	    {
    		if ((_poll_fds[i].revents & POLLIN) != 1)
	            continue ;
    		if (this->_poll_fds[i].fd == _server_socket)
			{
    			int client_fd = accept(_server_socket, NULL, NULL);
    			if (client_fd != -1) 
				{
    				this->add_to_poll_fds(client_fd);
					User tmp(client_fd);
					_users[client_fd] = tmp;
				}
			}
	        else 
	            handleMessage(i);
	    }
	}
}

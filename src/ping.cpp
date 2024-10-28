#include "../inc/server.hpp"

bool	Server::ping(User &client, std::string cmd)
{
    (void) cmd;
    int status;
    std::string serv_msg = "PONG localhost\r\n";
    status = send(client.getFd(), serv_msg.c_str(), strlen(serv_msg.c_str()), MSG_NOSIGNAL);
    if (status == -1)
        std::cout << "[Server] Send error to client " << client.getFd() << strerror(errno) << std::endl;
    return (1);
}

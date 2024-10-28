#include "../inc/server.hpp"
    
const char  *Server::init_failed::what(void) const throw()
{   
    return ("The server initialization failed.");
}

const char  *Server::select_failed::what(void) const throw()
{       
    return ("A call to select failed.");
}
        
const char  *Server::closing_server::what(void) const throw()
{   
    return ("\nThe server's closing.");
}   
              
const char  *Server::fatal_error::what(void) const throw()
{   
    return ("A fatal error occurred while running the server.");
}   

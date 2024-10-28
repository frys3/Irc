#include "../inc/irc.hpp"

const char  *invalid_args::what(void) const throw()
{   
    return ("The program needs, in this order :\n    _ a port number\n    _ a password\n");
}

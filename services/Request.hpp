#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "LibsAndClassDeclarations.h"


class Request
{
    std::string server_ip;
    int server_port;

public:
    Request(const std::string& server_ip,
            int server_port);

    std::string getServerIp() const;
    int getServerPort() const;
};

#endif

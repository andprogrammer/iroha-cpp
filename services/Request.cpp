#include "services/Request.hpp"


Request::Request(const std::string& server_ip,
                 int server_port)
    : server_ip(server_ip),
      server_port(server_port)
{}

std::string Request::getServerIp() const
{
    return server_ip;
}

int Request::getServerPort() const
{
    return server_port;
}

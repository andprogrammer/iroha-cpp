#include "services/Request.hpp"


Request::Request(const std::string& server_ip,
                 int server_port,
                 logger::LoggerPtr pb_qry_factory_log)
    : server_ip(server_ip),
      server_port(server_port),
      pb_qry_factory_log_(std::move(pb_qry_factory_log))
{}

std::string Request::getServerIp() const
{
    return server_ip;
}

int Request::getServerPort() const
{
    return server_port;
}

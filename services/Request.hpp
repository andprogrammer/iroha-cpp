#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "LibsAndClassDeclarations.h"

#include "logger/logger_fwd.hpp"


class Request
{
protected:
    std::string server_ip;
    int server_port;
    logger::LoggerPtr pb_qry_factory_log_;

public:
    Request(const std::string& server_ip,
            int server_port,
            logger::LoggerPtr pb_qry_factory_log);

    std::string getServerIp() const;
    int getServerPort() const;
};

#endif

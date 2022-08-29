#ifndef STATUS_HPP
#define STATUS_HPP

#include "LibsAndClassDeclarations.h"

#include <endpoint.pb.h>
#include "logger/logger_fwd.hpp"


namespace IROHA_CPP
{

class Status
{
    std::string tx_hash;
    logger::LoggerPtr pb_qry_factory_log_;

public:
    struct Response
    {
        std::string status_name;
        iroha::protocol::TxStatus status_code;
        uint32_t error_code;
    };

    Status(const std::string& tx_hash, logger::LoggerPtr pb_qry_factory_log);
    const Response getTxStatus(std::string server_ip, int server_port) const;
    const std::string getHash() const;
};

}

#endif

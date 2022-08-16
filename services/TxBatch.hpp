#ifndef TX_BATCH_HPP
#define TX_BATCH_HPP

#include "LibsAndClassDeclarations.h"

#include "logger/logger_manager_fwd.hpp"
#include "services/Request.hpp"
#include "transaction.pb.h"
#include <boost/bimap.hpp>
#include "crypto/keypair.hpp"
#include <endpoint.pb.h>


class TxBatch: public Request
{
    logger::LoggerManagerTreePtr response_handler_log_manager_;
    std::vector<iroha::protocol::Transaction> transactions_;

public:
    TxBatch(const std::string& server_ip,
            int server_port,
            logger::LoggerPtr pb_qry_factory_log,
            logger::LoggerManagerTreePtr response_handler_log_manager);

    void addTransaction(const iroha::protocol::Transaction& tx);
    const std::vector<std::string> send();
};

#endif

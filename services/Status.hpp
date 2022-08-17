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
    const std::map<iroha::protocol::TxStatus, std::string> userMessageMap =
    {
        {
            iroha::protocol::TxStatus::STATELESS_VALIDATION_FAILED, "Transaction has not passed stateless validation."
        },
        {
            iroha::protocol::TxStatus::STATELESS_VALIDATION_SUCCESS, "Transaction has successfully passed stateless validation."
        },
        {
            iroha::protocol::TxStatus::STATEFUL_VALIDATION_FAILED, "Transaction has not passed stateful validation."
        },
        {
            iroha::protocol::TxStatus::STATEFUL_VALIDATION_SUCCESS, "Transaction has successfully passed stateful validation."
        },
        {
            iroha::protocol::TxStatus::REJECTED, "Transaction has been rejected."
        },
        {
            iroha::protocol::TxStatus::COMMITTED, "Transaction was successfully committed."
        },
        {
            iroha::protocol::TxStatus::MST_EXPIRED, "Transaction has not collected enough signatures in time."
        },
        {
            iroha::protocol::TxStatus::NOT_RECEIVED, "Transaction was not found in the system."
        },
        {
            iroha::protocol::TxStatus::MST_PENDING, "Transaction has not collected quorum of signatures."
        },
        {
            iroha::protocol::TxStatus::ENOUGH_SIGNATURES_COLLECTED, "Transaction has collected all signatures."
        }
    };
    logger::LoggerPtr pb_qry_factory_log_;

public:
    Status(const std::string& tx_hash, logger::LoggerPtr pb_qry_factory_log);
    const std::string getTxStatus(std::string server_ip, int server_port) const;
    const std::string getHash() const;
};

}

#endif

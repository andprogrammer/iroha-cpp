#ifndef TRANSACTION_RESPONSE_HANDLER_HPP
#define TRANSACTION_RESPONSE_HANDLER_HPP

#include "GrpcClient.hpp"  // for GrpcClient::TxStatus (yuck!)

#include "logger/logger_fwd.hpp"


namespace spdlog
{
    class logger;
}


class TransactionResponseHandler
{
    logger::LoggerPtr log_;

public:
    explicit TransactionResponseHandler(logger::LoggerPtr log);
    void handle(const GrpcClient::TxStatus status) const;
};

#endif

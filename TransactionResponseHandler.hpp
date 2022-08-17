#ifndef TRANSACTION_RESPONSE_HANDLER_HPP
#define TRANSACTION_RESPONSE_HANDLER_HPP

#include "GrpcClient.hpp"

#include "logger/logger_fwd.hpp"


namespace IROHA_CPP
{


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

}

#endif

#include "TransactionResponseHandler.hpp"
#include "logger/logger.hpp"


namespace IROHA_CPP
{


void TransactionResponseHandler::handle(const GrpcClient::TxStatus status) const
{
    switch (status)
    {
    case GrpcClient::OK:
        log_->info("Transaction successfully sent");
        break;
    }
}

TransactionResponseHandler::TransactionResponseHandler(logger::LoggerPtr log)
    : log_(std::move(log))
{}

}

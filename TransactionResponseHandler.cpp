#include "TransactionResponseHandler.hpp"
#include "logger/logger.hpp"


void TransactionResponseHandler::handle(const GrpcClient::TxStatus status) const
{
    switch (status)
    {
    case GrpcClient::OK:
        log_->info("Transaction successfully sent");
        break;
        /*
      case GrpcClient::NOT_VALID:
        log_->error("Transaction is not valid");
        break;
         */
    }
}

TransactionResponseHandler::TransactionResponseHandler(logger::LoggerPtr log)
    : log_(std::move(log))
{}

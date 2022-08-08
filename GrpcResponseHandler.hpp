#ifndef GRPC_RESPONSE_HANDLER_HPP
#define GRPC_RESPONSE_HANDLER_HPP

#include "logger/logger_fwd.hpp"
#include "logger/logger_manager_fwd.hpp"
#include "QueryResponseHandler.hpp"
#include "TransactionResponseHandler.hpp"


namespace spdlog
{
    class logger;
}


class GrpcResponseHandler
{
    TransactionResponseHandler tx_handler_;
    QueryResponseHandler query_handler_;
    void handleGrpcErrors(grpc::StatusCode code);
    logger::LoggerPtr log_;
    std::unordered_map<int, std::string> handler_map_;

public:
    explicit GrpcResponseHandler(logger::LoggerManagerTreePtr log_manager);
    void handle(GrpcClient::Response<GrpcClient::TxStatus> response);
    void handle(GrpcClient::Response<iroha::protocol::QueryResponse> response);
};

#endif

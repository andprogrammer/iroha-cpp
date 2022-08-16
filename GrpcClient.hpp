#ifndef GRPC_CLIENT_HPP
#define GRPC_CLIENT_HPP

#include <string>

#include "logger/logger_fwd.hpp"
#include "torii/command_client.hpp"
#include "torii/query_client.hpp"


class GrpcClient
{
    torii::CommandSyncClient command_client_;
    torii_utils::QuerySyncClient query_client_;
    logger::LoggerPtr pb_qry_factory_log_;

public:
    template <typename T>
    struct Response
    {
        grpc::Status status;
        T answer;
    };

    // TODO 13/09/17 luckychess: check if we need more status codes IR-494
    enum TxStatus { OK };

    GrpcClient(std::string target_ip, int port, logger::LoggerPtr pb_qry_factory_log);
    GrpcClient::Response<GrpcClient::TxStatus> sendTx(const iroha::protocol::Transaction& tx);
    GrpcClient::Response<GrpcClient::TxStatus> sendTxList(const iroha::protocol::TxList& tx_list);
    GrpcClient::Response<iroha::protocol::QueryResponse> sendQuery(const iroha::protocol::Query& query);
    GrpcClient::Response<iroha::protocol::ToriiResponse> getTxStatus(const std::string& tx_hash);
};

#endif

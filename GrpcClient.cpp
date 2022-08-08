#include "GrpcClient.hpp"

#include "backend/protobuf/queries/proto_query.hpp"
#include "backend/protobuf/transaction.hpp"
#include "model/converters/json_query_factory.hpp"
#include "model/converters/json_transaction_factory.hpp"
#include "model/converters/pb_query_factory.hpp"
#include "model/converters/pb_transaction_factory.hpp"
#include "network/impl/channel_factory.hpp"


template <typename Service>
std::unique_ptr<typename Service::StubInterface> makeStub(const std::string& target_ip, int port)
{
    using namespace iroha::network;
    return createInsecureClient<Service>(target_ip, port, std::nullopt);
}

GrpcClient::GrpcClient(std::string target_ip,
                       int port,
                       logger::LoggerPtr pb_qry_factory_log)
    : command_client_(makeStub<torii::CommandSyncClient::Service>(target_ip, port), pb_qry_factory_log),
      query_client_(makeStub<torii_utils::QuerySyncClient::Service>(target_ip, port)),
      pb_qry_factory_log_(std::move(pb_qry_factory_log))
{}

GrpcClient::Response<GrpcClient::TxStatus> GrpcClient::sendTx(const iroha::protocol::Transaction& tx)
{
    GrpcClient::Response<GrpcClient::TxStatus> response;
    // Send to iroha:
    response.status = command_client_.Torii(tx);
    // TODO 12/10/2017 neewy implement return of real transaction status IR-494
    response.answer = TxStatus::OK;
    return response;
}

GrpcClient::Response<iroha::protocol::ToriiResponse> GrpcClient::getTxStatus(const std::string& tx_hash)
{
    GrpcClient::Response<iroha::protocol::ToriiResponse> response;
    // Send to iroha:
    iroha::protocol::TxStatusRequest statusRequest;
    statusRequest.set_tx_hash(tx_hash);
    iroha::protocol::ToriiResponse toriiResponse;
    response.status = command_client_.Status(statusRequest, toriiResponse);
    response.answer = toriiResponse;
    return response;
}

GrpcClient::Response<iroha::protocol::QueryResponse> GrpcClient::sendQuery(const iroha::protocol::Query& query)
{
    GrpcClient::Response<iroha::protocol::QueryResponse> response;
    iroha::model::converters::PbQueryFactory pb_factory(pb_qry_factory_log_);
    iroha::protocol::QueryResponse query_response;
    response.status = query_client_.Find(query, query_response);    // raczej ok
    response.answer = query_response;
    return response;
}
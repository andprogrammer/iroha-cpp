#include "GrpcClient.hpp"


namespace iroha_lib
{

template <typename Service>
std::shared_ptr<grpc::Channel> createChanel(const std::string& address)
{
    return grpc::CreateChannel(address, grpc::InsecureChannelCredentials());
}


template <typename Service>
std::unique_ptr<typename Service::StubInterface> createClient(const std::string& address)
{
    return Service::NewStub(createChanel<Service>(address));
}


template <typename Service>
std::unique_ptr<typename Service::StubInterface> createClient(const std::string& ip, const size_t port)
{
    const auto peer_ip = ip + ":" + std::to_string(port);
    return createClient<Service>(peer_ip);
}


GrpcClient::GrpcClient(const std::string& target_ip, const uint16_t port)
    : commandStub(std::move(createClient<iroha::protocol::CommandService_v1>(target_ip, port))),
      queryStub(std::move(createClient<iroha::protocol::QueryService_v1>(target_ip, port)))
{}

grpc::Status GrpcClient::send(const iroha::protocol::Transaction& tx)
{
    google::protobuf::Empty empty;
    grpc::ClientContext context;
    return commandStub->Torii(&context, tx, &empty);
}

grpc::Status GrpcClient::send(const iroha::protocol::TxList& tx_list)
{
    google::protobuf::Empty empty;
    grpc::ClientContext context;
    return commandStub->ListTorii(&context, tx_list, &empty);
}

iroha::protocol::QueryResponse GrpcClient::send(const iroha::protocol::Query& query)
{
    iroha::protocol::QueryResponse queryResponse;
    grpc::ClientContext context;
    queryStub->Find(&context, query, &queryResponse);
    return queryResponse;
}

iroha::protocol::ToriiResponse GrpcClient::getTxStatus(const std::string& tx_hash)
{
    iroha::protocol::TxStatusRequest statusRequest;
    statusRequest.set_tx_hash(tx_hash);
    iroha::protocol::ToriiResponse toriiResponse;
    grpc::ClientContext context;
    commandStub->Status(&context, statusRequest, &toriiResponse);
    return toriiResponse;
}

}

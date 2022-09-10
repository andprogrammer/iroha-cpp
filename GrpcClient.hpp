#ifndef GRPC_CLIENT_HPP
#define GRPC_CLIENT_HPP

#include <endpoint.grpc.pb.h>
#include <grpc++/grpc++.h>


namespace iroha_lib
{

class GrpcClient
{
public:
    GrpcClient(const std::string& target_ip, const uint16_t port);
    grpc::Status send(const iroha::protocol::Transaction& tx);
    grpc::Status send(const iroha::protocol::TxList& tx_list);
    iroha::protocol::QueryResponse send(const iroha::protocol::Query& query);
    iroha::protocol::ToriiResponse getTxStatus(const std::string& tx_hash);

private:
    std::shared_ptr<iroha::protocol::CommandService_v1::StubInterface> commandStub;
    std::shared_ptr<iroha::protocol::QueryService_v1::StubInterface> queryStub;
};

}

#endif

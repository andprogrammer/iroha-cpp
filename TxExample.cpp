#include <iostream>

#include "model/utils/Utils.h"


void sendSampleTransaction(const std::string& account_name,
                           const std::string& key_path,
                           const std::string& peer_ip,
                           uint16_t torii_port,
                           uint32_t quorum,
                           const std::string& domain_id,
                           const std::string& user_default_role,
                           const std::string& asset_name)
{
    const auto tx_proto = generateTransaction(account_name,
                                              key_path,
                                              peer_ip,
                                              torii_port,
                                              quorum,
                                              domain_id,
                                              user_default_role,
                                              asset_name);

    iroha_lib::GrpcClient(peer_ip, torii_port).send(tx_proto);

    const auto tx_hash = getTransactionHash(tx_proto);
    printTransactionStatus(peer_ip, torii_port, tx_hash);
}


void run()
{
    auto account_name = "admin@test";
    const auto key_path = "/home/laptop/qt-workspace/iroha/example";
    const auto peer_ip = "127.0.0.1";
    uint16_t torii_port = 50051;
    const auto quorum = 1;
    const auto user_default_role = "user";

    sendSampleTransaction(account_name,
                          key_path,
                          peer_ip,
                          torii_port,
                          quorum,
                          "domainsamplev1",
                          user_default_role,
                          "assetnamesamplev1");
}


int main()
{
    run();
    return 0;
}

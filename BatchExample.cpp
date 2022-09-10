#include <iostream>

#include "model/utils/Utils.h"
#include "model/TxBatch.hpp"


void sendSampleBatchTransaction(const std::string& account_name,
                                const std::string& key_path,
                                const std::string& peer_ip,
                                uint16_t torii_port,
                                uint32_t quorum,
                                const std::string& user_default_role)
{
    const auto tx_a = generateTransaction(account_name,
                                          key_path,
                                          peer_ip,
                                          torii_port,
                                          quorum,
                                          "domainsamplev2",
                                          user_default_role,
                                          "assetnamesamplev2");
    const auto tx_b = generateTransaction(account_name,
                                          key_path,
                                          peer_ip,
                                          torii_port,
                                          quorum,
                                          "domainsamplev3",
                                          user_default_role,
                                          "assetnamesamplev3");

    iroha_lib::TxBatch tx_batch;
    std::vector<iroha::protocol::Transaction> transactions({tx_a, tx_b});
    iroha_lib::GrpcClient(peer_ip, torii_port).send(tx_batch.batch(transactions));
    printTransactionStatuses(peer_ip, torii_port, transactions);
}


void run()
{
    auto account_name = "admin@test";
    const auto key_path = "/home/laptop/qt-workspace/iroha/example";
    const auto peer_ip = "127.0.0.1";
    uint16_t torii_port = 50051;
    const auto quorum = 1;
    const auto user_default_role = "user";

    sendSampleBatchTransaction(account_name,
                               key_path,
                               peer_ip,
                               torii_port,
                               quorum,
                               user_default_role);
}


int main()
{
    run();
    return 0;
}

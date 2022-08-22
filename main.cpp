#include <boost/filesystem.hpp>
#include <iostream>
#include <cassert>

#include "crypto/keys_manager_impl.hpp"
#include "logger/logger_manager.hpp"
#include "model/converters/pb_common.hpp"

#include "services/Query.hpp"
#include "services/Status.hpp"
#include "services/Tx.hpp"
#include "services/TxBatch.hpp"
#include "GrpcResponseHandler.hpp"


namespace fs = boost::filesystem;


auto log_manager = std::make_shared<logger::LoggerManagerTree>(logger::LoggerConfig{logger::LogLevel::kInfo, logger::getDefaultLogPatterns()})->getChild("CLI");
const auto response_handler_log_manager = log_manager->getChild("ResponseHandler");
const auto pb_qry_factory_log = log_manager->getChild("PbQueryFactory")->getLogger();

const auto TX_COLLECTED_SIG_STATUS = "Transaction has collected all signatures.";


int verifyPath(const fs::path& path, const logger::LoggerPtr& logger)
{
    if (not fs::exists(path))
    {
        logger->error("Path {} not found.", path.string());
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}


int verifyKepair(const iroha::expected::Result<shared_model::crypto::Keypair, std::string>& keypair,
                 const logger::LoggerPtr& logger,
                 const fs::path& path,
                 const std::string& account_name)
{
    if (auto e = iroha::expected::resultToOptionalError(keypair))
    {
        logger->error("Keypair error: {}.\n"
                      "Keypair path: '{}', name: {}.\n",
                      e.value(),
                      path.string(),
                      account_name);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}


auto generateKeypair(const std::string& account_name,
                     const std::string& key_path,
                     const logger::LoggerManagerTreePtr& log_manager)
{
    const auto logger = log_manager->getChild("Main")->getLogger();
    const auto keys_manager_log = log_manager->getChild("KeysManager")->getLogger();
    fs::path path(key_path);

    verifyPath(path, logger);

    iroha::KeysManagerImpl manager((path / account_name).string(), keys_manager_log);
    auto keypair = manager.loadKeys(boost::none);

    verifyKepair(keypair, logger, path, account_name);
    return iroha::keypair_t(iroha::pubkey_t::from_hexstring(keypair.assumeValue().publicKey()).assumeValue(),
                            iroha::privkey_t::from_string(toBinaryString(keypair.assumeValue().privateKey())).assumeValue());
}


const std::string getTransactionHash(const iroha::protocol::Transaction& tx)
{
    return iroha::hash(tx).to_hexstring();
}


int verifyTransactionStatus(const std::string& peer_ip,
                            int torii_port,
                            const std::string& tx_hash)
{
    IROHA_CPP::Status status(tx_hash, pb_qry_factory_log);
    const auto statusResponse = status.getTxStatus(peer_ip, torii_port);
    std::cout << "Tx status=" << statusResponse << std::endl;
    return statusResponse.find(TX_COLLECTED_SIG_STATUS) != std::string::npos ? EXIT_SUCCESS : EXIT_FAILURE;
}


iroha::protocol::Transaction generateTransaction(const std::string& account_name,
                                                 const std::string& key_path,
                                                 const std::string& peer_ip,
                                                 int torii_port,
                                                 uint32_t quorum,
                                                 const std::string& domain_id,
                                                 const std::string& user_default_role,
                                                 const std::string& asset_name)
{
    const auto keypair = generateKeypair(account_name, key_path, log_manager);

    IROHA_CPP::Tx tx(account_name, keypair);

    const auto tx_proto = tx.createDomain(domain_id, user_default_role)
            .createAsset(asset_name, domain_id, 0)
            .signAndAddSignature();

    const auto tx_hash = getTransactionHash(tx_proto);
    std::cout << "Tx hash=" << tx_hash << std::endl;

    return tx_proto;
}


iroha::protocol::Transaction sendSampleTransaction(const std::string& account_name, const std::string& key_path, const std::string& peer_ip, int torii_port, uint32_t quorum, const std::string& domain_id, const std::string& user_default_role, const std::string& asset_name)
{
    const auto tx_proto = generateTransaction(account_name, key_path, peer_ip, torii_port, quorum, domain_id, user_default_role, asset_name);
    IROHA_CPP::GrpcResponseHandler response_handler(response_handler_log_manager);
    response_handler.handle(IROHA_CPP::GrpcClient(peer_ip, torii_port, pb_qry_factory_log).send(tx_proto));
    const auto tx_hash = getTransactionHash(tx_proto);
    std::cout << "Tx hash=" << tx_hash << std::endl;
    return tx_proto;
}


int verifyTransactionStatuses(const std::string& peer_ip,
                              int torii_port,
                              const std::vector<std::string>& tx_hashes)
{
    for (const auto& hash: tx_hashes)
        if (verifyTransactionStatus(peer_ip, torii_port, hash) != EXIT_SUCCESS)
            return EXIT_FAILURE;
    return EXIT_SUCCESS;
}


void sendBatchTransaction(const std::string& peer_ip, int torii_port, const IROHA_CPP::TxBatch& tx_batch)
{
    IROHA_CPP::GrpcResponseHandler response_handler(response_handler_log_manager);
    response_handler.handle(IROHA_CPP::GrpcClient(peer_ip, torii_port, pb_qry_factory_log).send(tx_batch.getTxList()));
    assert(EXIT_SUCCESS == verifyTransactionStatuses(peer_ip, torii_port, tx_batch.getTxHashes()));
}


int sendSampleBatchTransaction(const std::string& account_name, const std::string& key_path, const std::string& peer_ip, int torii_port, uint32_t quorum, const std::string& user_default_role)
{
    const auto tx_a = generateTransaction(account_name, key_path, peer_ip, torii_port, quorum, "domainsamplev2", user_default_role, "assetnamesamplev2");
    const auto tx_b = generateTransaction(account_name, key_path, peer_ip, torii_port, quorum, "domainsamplev3", user_default_role, "assetnamesamplev3");

    IROHA_CPP::TxBatch tx_batch;
    tx_batch.addTransaction(tx_a);
    tx_batch.addTransaction(tx_b);
    sendBatchTransaction(peer_ip, torii_port, tx_batch);
    return EXIT_SUCCESS;
}


int run()
{
    auto account_name = "admin@test";
    const auto key_path = "/home/laptop/qt-workspace/iroha/example";
    const auto peer_ip = "127.0.0.1";
    int torii_port = 50051;
    const auto quorum = 1;
    const auto user_default_role = "user";

    sendSampleTransaction(account_name, key_path, peer_ip, torii_port, quorum, "domainsamplev1", user_default_role, "assetnamesamplev1");
    sendSampleBatchTransaction(account_name, key_path, peer_ip, torii_port, quorum, user_default_role);
    return EXIT_SUCCESS;
}


int main()
{
    return run();
}

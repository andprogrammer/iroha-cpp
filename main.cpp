#include <boost/filesystem.hpp>
#include <iostream>
#include <cassert>

#include "crypto/keys_manager_impl.hpp"
#include "logger/logger_manager.hpp"

#include "services/Query.hpp"
#include "services/Status.hpp"
#include "services/Tx.hpp"
#include "services/TxBatch.hpp"


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


Tx generateTx(const std::string& account_name,
              const std::string& key_path,
              const std::string& peer_ip,
              int torii_port,
              uint32_t quorum,
              const std::string& domain_id,
              const std::string& user_default_role,
              const std::string& asset_name)
{
    const auto keypair = generateKeypair(account_name, key_path, log_manager);

    Tx tx(account_name,
          peer_ip,
          torii_port,
          response_handler_log_manager,
          pb_qry_factory_log,
          keypair);

    const auto tx_hash = tx.createDomain(domain_id, user_default_role)
            .createAsset(asset_name, domain_id, 0)
            .signAndAddSignature()
            .send();
    std::cout << "Tx hash=" << tx_hash << std::endl;

    Status status(peer_ip,
                  torii_port,
                  tx_hash,
                  pb_qry_factory_log);
    std::cout << "Tx status=" << status.getTxStatus() << std::endl;
    return tx;
}


int verifyTxStatus(const std::string& peer_ip,
                   int torii_port,
                   const std::string& tx_hash)
{
    Status status(peer_ip,
                  torii_port,
                  tx_hash,
                  pb_qry_factory_log);
    return status.getTxStatus().find(TX_COLLECTED_SIG_STATUS) != std::string::npos ? EXIT_SUCCESS : EXIT_FAILURE;
}


int verifyTxStatuses(const std::string& peer_ip,
                     int torii_port,
                     const std::vector<std::string>& tx_hashes)
{
    for (const auto& hash: tx_hashes)
        if (verifyTxStatus(peer_ip, torii_port, hash) != EXIT_SUCCESS)
            return EXIT_FAILURE;
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

    const auto tx_a = generateTx(account_name, key_path, peer_ip, torii_port, quorum, "domainsamplev4", user_default_role, "assetnamesamplev4");
    const auto tx_b = generateTx(account_name, key_path, peer_ip, torii_port, quorum, "domainsamplev5", user_default_role, "assetnamesamplev5");

    TxBatch tx_batch(peer_ip,
                     torii_port,
                     pb_qry_factory_log,
                     response_handler_log_manager);

    tx_batch.addTransaction(tx_a.getTx());
    tx_batch.addTransaction(tx_b.getTx());
    std::vector<std::string> tx_hashes = tx_batch.send();
    assert(EXIT_SUCCESS == verifyTxStatuses(peer_ip, torii_port, tx_hashes));
    return 0;
}


int main()
{
    return run();
}

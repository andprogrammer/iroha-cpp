#include <boost/filesystem.hpp>
#include <iostream>
#include <fmt/core.h>

#include "crypto/keys_manager_impl.hpp"
#include "logger/logger_manager.hpp"
#include "model/converters/pb_common.hpp"

#include "model/Tx.hpp"
#include "model/TxBatch.hpp"
#include "GrpcClient.hpp"
#include "Utils.h"


namespace fs = boost::filesystem;


void verifyPath(const fs::path& path, const logger::LoggerPtr& logger)
{
    if (not fs::exists(path))
    {
        const auto error_message = "Path " + path.string() + " not found.";
        logger->error(error_message);
        throw error_message;
    }
}


void verifyKepair(const iroha::expected::Result<shared_model::crypto::Keypair, std::string>& keypair,
                  const logger::LoggerPtr& logger,
                  const fs::path& path,
                  const std::string& account_name)
{
    if (auto error = iroha::expected::resultToOptionalError(keypair))
    {
        const auto error_message = fmt::format("Keypair error= {}."
                                               "\nKeypair path= {}, name= {}.\n",
                                               error.value(),
                                               path.string(),
                                               account_name);
        logger->error(error_message);
        throw error_message;
    }
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


void printTransactionStatus(const std::string& peer_ip,
                            uint16_t torii_port,
                            const std::string& tx_hash)
{
    const auto status = iroha_lib::GrpcClient(peer_ip, torii_port).getTxStatus(tx_hash);
    std::cout << "Tx hash=" << tx_hash
              << "  Status name=" << TxStatus_Name(status.tx_status())
              << "  Status code=" << status.tx_status()
              << "  Error code=" << status.error_code()
              << std::endl;
}


iroha::protocol::Transaction generateTransaction(const std::string& account_name,
                                                 const std::string& key_path,
                                                 const std::string& peer_ip,
                                                 uint16_t torii_port,
                                                 uint32_t quorum,
                                                 const std::string& domain_id,
                                                 const std::string& user_default_role,
                                                 const std::string& asset_name)
{
    auto log_manager = std::make_shared<logger::LoggerManagerTree>(logger::LoggerConfig{logger::LogLevel::kInfo, logger::getDefaultLogPatterns()})->getChild("CLI");
    const auto keypair = generateKeypair(account_name, key_path, log_manager);

    iroha_lib::Tx tx(account_name, keypair);

    const auto tx_proto = tx.createDomain(domain_id, user_default_role)
            .createAsset(asset_name, domain_id, 0)
            .signAndAddSignature();
    return tx_proto;
}


void printTransactionStatuses(const std::string& peer_ip,
                              uint16_t torii_port,
                              const std::vector<iroha::protocol::Transaction>& transactions)
{
    for (const auto& tx: transactions)
        printTransactionStatus(peer_ip, torii_port, getTransactionHash(tx));
}

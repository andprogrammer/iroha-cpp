#include <boost/filesystem.hpp>
#include <iostream>

#include "crypto/keys_manager_impl.hpp"
#include "logger/logger_manager.hpp"

#include "services/Query.hpp"
#include "services/Status.hpp"
#include "services/Tx.hpp"
#include "datetime/time.hpp"


namespace fs = boost::filesystem;


iroha::keypair_t *makeOldModel(const shared_model::crypto::Keypair &keypair)
{
    return new iroha::keypair_t
    {
        iroha::pubkey_t::from_hexstring(keypair.publicKey()).assumeValue(),
                iroha::privkey_t::from_string(toBinaryString(keypair.privateKey()))
                .assumeValue()
    };
}


int main()
{
    auto log_manager = std::make_shared<logger::LoggerManagerTree>(logger::LoggerConfig{logger::LogLevel::kInfo, logger::getDefaultLogPatterns()})->getChild("CLI");
    const auto logger = log_manager->getChild("Main")->getLogger();
    const auto response_handler_log_manager = log_manager->getChild("ResponseHandler");
    const auto pb_qry_factory_log = log_manager->getChild("PbQueryFactory")->getLogger();
    const auto keys_manager_log = log_manager->getChild("KeysManager")->getLogger();

    std::string account_name = "admin@test";                            // Name of the account. Must be unique in iroha network
    std::string key_path = "/home/laptop/qt-workspace/iroha/example";   // Path to user key
    std::string peer_ip = "127.0.0.1";                                  // Address of the Iroha node
    int torii_port = 50051;                                             // Port of Iroha's Torii
    std::string pass_phrase = "";                                       // Account pass-phrase

    fs::path path(key_path);

    if (not fs::exists(path))
    {
        logger->error("Path {} not found.", path.string());
        return EXIT_FAILURE;
    }

    iroha::KeysManagerImpl manager((path / account_name).string(), keys_manager_log);
    auto keypair = pass_phrase.size() != 0 ? manager.loadKeys(pass_phrase) : manager.loadKeys(boost::none);

    if (auto e = iroha::expected::resultToOptionalError(keypair))
    {
        logger->error("Keypair error: {}.\n"
                      "keypair path: '{}', name: {}.\n"
                      "Maybe wrong pass phrase (\"{}\")?",
                      e.value(),
                      path.string(),
                      account_name,
                      pass_phrase);
        return EXIT_FAILURE;
    }

    Tx tx(account_name,
          peer_ip,
          torii_port,
          response_handler_log_manager,
          pb_qry_factory_log,
          *std::unique_ptr<iroha::keypair_t>(makeOldModel(keypair.assumeValue())),
          iroha::time::now(),
          1);

    std::string domain_id = "domainsamplev1";
    std::string user_default_role = "user";
    std::string asset_name = "assetnamesamplev1";
    tx.createDomain(domain_id, user_default_role)
            .createAsset(asset_name, domain_id, 0)
            .signAndAddSignature()
            .send();

//    Status status(peer_ip,
//                  torii_port,
//                  "4e218255ec26d00820d22363aad3274e63f60ce464bd58b5fe2f0c3c1261ac8a",
//                  pb_qry_factory_log);
//    std::cout << "tx status= " << status.getTxStatus() << std::endl;

    return 0;
}

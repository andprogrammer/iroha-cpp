#include <boost/filesystem.hpp>
#include <iostream>

#include "crypto/keys_manager_impl.hpp"
#include "logger/logger_manager.hpp"
#include "model/converters/pb_common.hpp"
#include "GrpcClient.hpp"


namespace fs = boost::filesystem;


void verifyPath(const fs::path& path, const logger::LoggerPtr& logger);

void verifyKepair(const iroha::expected::Result<shared_model::crypto::Keypair, std::string>& keypair,
                  const logger::LoggerPtr& logger,
                  const fs::path& path,
                  const std::string& account_name);

auto generateKeypair(const std::string& account_name,
                     const std::string& key_path,
                     const logger::LoggerManagerTreePtr& log_manager);

const std::string getTransactionHash(const iroha::protocol::Transaction& tx);

void printTransactionStatus(const std::string& peer_ip,
                            uint16_t torii_port,
                            const std::string& tx_hash);

iroha::protocol::Transaction generateTransaction(const std::string& account_name,
                                                 const std::string& key_path,
                                                 const std::string& peer_ip,
                                                 uint16_t torii_port,
                                                 uint32_t quorum,
                                                 const std::string& domain_id,
                                                 const std::string& user_default_role,
                                                 const std::string& asset_name);

void printTransactionStatuses(const std::string& peer_ip,
                              uint16_t torii_port,
                              const std::vector<iroha::protocol::Transaction>& transactions);

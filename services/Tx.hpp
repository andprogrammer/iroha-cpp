#ifndef TX_HPP
#define TX_HPP

#include "LibsAndClassDeclarations.h"

#include "logger/logger_fwd.hpp"
#include "logger/logger_manager_fwd.hpp"
#include "services/Request.hpp"
#include "transaction.pb.h"
#include <boost/bimap.hpp>
#include "crypto/keypair.hpp"


class Tx: public Request
{
    std::string creator_;
    logger::LoggerManagerTreePtr response_handler_log_manager_;
    logger::LoggerPtr pb_qry_factory_log_;
    iroha::keypair_t keypair_;
    iroha::protocol::Transaction pbtx;

private:
    boost::bimap<iroha::protocol::RolePermission, std::string> pb_role_map_;
    boost::bimap<iroha::protocol::GrantablePermission, std::string> pb_grant_map_;

public:
    Tx(const std::string& creator_account,
       const std::string& server_ip,
       int server_port,
       logger::LoggerManagerTreePtr response_handler_log_manager,
       logger::LoggerPtr pb_qry_factory_log,
       const iroha::keypair_t& keypair,
       uint64_t created_time,
       uint32_t quorum);
    void addCommand(const iroha::protocol::Command& command);
    Tx& createRole(const std::string& role_name, const std::set<std::string>& permissions);
    Tx& appendRole(const std::string& account_id, const std::string& role_name);
    Tx& detachRole(const std::string& account_id, const std::string& role_name);
    Tx& grantPermission(const std::string& account_id, const std::string& permission_name);
    Tx& revokePermission(const std::string& account_id, const std::string& permission_name);
    Tx& addAssetQuantity(const std::string& account_id, const std::string& role_name);
    Tx& subtractAssetQuantity(const std::string& asset_id, const std::string& amount);
    Tx& addPeer(const std::string& address, const std::string& pubkey);
    Tx& addSignatory(const std::string& account_id, const std::string& pubkey);
    //    Tx& addSignatory(const std::string& account_id, const std::string&& pubkey);  // alternative impl
    Tx& createAsset(const std::string& asset_name, const std::string& domain_id, uint32_t precision);
    //    Tx& createAccount(const std::string& account_id, const std::string& domain_id, const std::string&& pubkey);  // alternative impl
    Tx& createAccount(const std::string& account_id, const std::string& domain_id, const std::string& pubkey);
    Tx& setAccountDetail(const std::string& account_id, const std::string& key, const std::string& value);
    Tx& createDomain(const std::string& domain_id, const std::string& user_default_role);
    Tx& removeSignatory(const std::string& account_id, const std::string& pubkey);
    //    Tx& removeSignatory(const std::string& account_id);   // alternative impl
    Tx& setAccountQuorum(const std::string& account_id, uint32_t quorum);
    Tx& transferAsset(const std::string& account_id,
                      const std::string& dest_account_id,
                      const std::string& asset_id,
                      const std::string& description,
                      const std::string& amount);

    Tx& signAndAddSignature();
    bool send();
    void printTransactionHash(iroha::protocol::Transaction& tx) const;

private:
    void populateRoleMap();
    void populateGrantMap();
};

#endif

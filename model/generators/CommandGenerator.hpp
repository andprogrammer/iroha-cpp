#ifndef COMMAND_GENERATOR_HPP
#define COMMAND_GENERATOR_HPP

#include "commands.pb.h"


namespace iroha_lib
{

class CommandGenerator
{
public:
    template <typename Type, typename... ParamTypes>
    std::shared_ptr<iroha::protocol::Command> generateCommand(ParamTypes... args)
    {
        return std::make_shared<Type>(args...);
    }

    std::shared_ptr<iroha::protocol::Command> generateAddAssetQuantity(const std::string &asset_id,
                                                                       const std::string &amount);
    std::shared_ptr<iroha::protocol::Command> generateAddPeer(const std::string& address,
                                                              const std::string& pubkey,
                                                              const std::optional<std::string>& tls_certificate,
                                                              bool syncing_peer);
    std::shared_ptr<iroha::protocol::Command> generateAddSignatory(const std::string& account_id,
                                                                   const std::string& pubkey);
    std::shared_ptr<iroha::protocol::Command> generateAppendRole(const std::string& account_id,
                                                                 const std::string& role_name);
    std::shared_ptr<iroha::protocol::Command> generateCreateAccount(const std::string& account_name,
                                                                    const std::string& domain_id,
                                                                    const std::string& pubkey);     // pubkey_t
    std::shared_ptr<iroha::protocol::Command> generateCreateAsset(const std::string &asset_name,
                                                                  const std::string &domain_name,
                                                                  uint8_t precision);
    std::shared_ptr<iroha::protocol::Command> generateCreateDomain(const std::string &domain_id, const std::string &default_role);
    std::shared_ptr<iroha::protocol::Command> generateCreateRole(const std::string& roleName,
                                                                 const std::unordered_set<iroha::protocol::RolePermission>& permissions);
    std::shared_ptr<iroha::protocol::Command> generateDetachRole(const std::string& account_id,
                                                                 const std::string& role_name);
    std::shared_ptr<iroha::protocol::Command> generateGrantPermission(const std::string& account_id,
                                                                      const iroha::protocol::GrantablePermission permission);
    std::shared_ptr<iroha::protocol::Command> generateRemovePeer(const std::string& pubkey);
    std::shared_ptr<iroha::protocol::Command> generateRemoveSignatory(const std::string& account_id, const std::string& pubkey);   // pubkey_t
    std::shared_ptr<iroha::protocol::Command> generateRevokePermission(const std::string& account_id,
                                                                       const iroha::protocol::GrantablePermission permission);
    std::shared_ptr<iroha::protocol::Command> generateSetAccountDetail(const std::string& account_id,
                                                                       const std::string& key,
                                                                       const std::string& value);
    std::shared_ptr<iroha::protocol::Command> generateSetAccountQuorum(const std::string& account_id, uint32_t quorum);
    std::shared_ptr<iroha::protocol::Command> generateSubtractAssetQuantity(const std::string &asset_id,
                                                                            const std::string &amount);
    std::shared_ptr<iroha::protocol::Command> generateTransferAsset(const std::string& account_id,
                                                                    const std::string& dest_account_id,
                                                                    const std::string& asset_id,
                                                                    const std::string& description,
                                                                    const std::string& amount);
    std::shared_ptr<iroha::protocol::Command> generateCompareAndSetAccountDetail(const std::string& account_id,
                                                                                 const std::string& key,
                                                                                 const std::string& value,
                                                                                 const std::optional<std::string>& old_value,
                                                                                 bool check_empty);

};

}

#endif

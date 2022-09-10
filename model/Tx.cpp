#include "Tx.hpp"

#include <fstream>
#include <utility>

#include "GrpcClient.hpp"
#include "model/converters/json_transaction_factory.hpp"
#include "model/converters/pb_common.hpp"
#include "model/permissions.hpp"
#include "cryptography/ed25519_sha3_impl/internal/ed25519_impl.hpp"
#include <boost/assign/list_inserter.hpp>


namespace iroha_lib
{

Tx::Tx(const std::string& account_id,
       const iroha::keypair_t& keypair,
       uint64_t created_time,
       uint32_t quorum)
    : keypair(keypair)
{
    auto payload = protobufTransaction.mutable_payload()->mutable_reduced_payload();
    payload->set_created_time(created_time);
    payload->set_creator_account_id(account_id);
    payload->set_quorum(quorum);
}

void Tx::addCommand(const iroha::protocol::Command& command)
{
    auto payload = protobufTransaction.mutable_payload()->mutable_reduced_payload();
    auto cmd = payload->add_commands();
    new (cmd)
            iroha::protocol::Command(command);
}

Tx& Tx::addAssetQuantity(const std::string& asset_id, const std::string& amount)
{
    auto cmd = cmdGenerator.generateAddAssetQuantity(asset_id, amount);
    addCommand(*cmd);
    return *this;
}

Tx& Tx::addPeer(const std::string& address,
                const std::string& pubkey,
                const std::optional<std::string>& tls_certificate,
                bool syncing_peer)
{
    auto cmd = cmdGenerator.generateAddPeer(address, pubkey, tls_certificate, syncing_peer);
    addCommand(*cmd);
    return *this;
}

Tx& Tx::addSignatory(const std::string& account_id, const std::string& pubkey)
{
    auto cmd = cmdGenerator.generateAddSignatory(account_id, pubkey);
    addCommand(*cmd);
    return *this;
}

Tx& Tx::appendRole(const std::string& account_id, const std::string& role_name)
{
    auto cmd = cmdGenerator.generateAppendRole(account_id, role_name);
    addCommand(*cmd);
    return *this;
}

Tx& Tx::createAccount(const std::string& account_name,
                      const std::string& domain_id,
                      const std::string& pubkey)
{
    auto cmd = cmdGenerator.generateCreateAccount(account_name, domain_id, pubkey);
    addCommand(*cmd);
    return *this;
}

Tx& Tx::createAsset(const std::string& asset_name,
                    const std::string& domain_id,
                    uint32_t precision)
{
    auto cmd = cmdGenerator.generateCreateAsset(asset_name, domain_id, precision);
    addCommand(*cmd);
    return *this;
}

Tx& Tx::createDomain(const std::string& domain_id, const std::string& user_default_role)
{
    auto cmd = cmdGenerator.generateCreateDomain(domain_id, user_default_role);
    addCommand(*cmd);
    return *this;
}

Tx& Tx::createRole(const std::string& roleName,
                   const std::unordered_set<iroha::protocol::RolePermission>& permissions)
{
    auto cmd = cmdGenerator.generateCreateRole(roleName, permissions);
    addCommand(*cmd);
    return *this;
}

Tx& Tx::detachRole(const std::string& account_id, const std::string& role_name)
{
    auto cmd = cmdGenerator.generateDetachRole(account_id, role_name);
    addCommand(*cmd);
    return *this;
}

Tx& Tx::grantPermission(const std::string& account_id,
                        const iroha::protocol::GrantablePermission permission)
{
    auto cmd = cmdGenerator.generateGrantPermission(account_id, permission);
    addCommand(*cmd);
    return *this;
}

Tx& Tx::removePeer(const std::string& pubkey)
{
    auto cmd = cmdGenerator.generateRemovePeer(pubkey);
    addCommand(*cmd);
    return *this;
}

Tx& Tx::revokePermission(const std::string& account_id,
                         const iroha::protocol::GrantablePermission permission)
{
    auto cmd = cmdGenerator.generateGrantPermission(account_id, permission);
    addCommand(*cmd);
    return *this;
}

Tx& Tx::setAccountDetail(const std::string& account_id,
                         const std::string& key,
                         const std::string& value)
{
    auto cmd = cmdGenerator.generateSetAccountDetail(account_id, key, value);
    addCommand(*cmd);
    return *this;
}

Tx& Tx::setAccountQuorum(const std::string& account_id,
                         uint32_t quorum)
{
    auto cmd = cmdGenerator.generateSetAccountQuorum(account_id, quorum);
    addCommand(*cmd);
    return *this;
}

Tx& Tx::subtractAssetQuantity(const std::string& asset_id, const std::string& amount)
{
    auto cmd = cmdGenerator.generateSubtractAssetQuantity(asset_id, amount);
    addCommand(*cmd);
    return *this;
}

Tx& Tx::transferAsset(const std::string& account_id,
                      const std::string& dest_account_id,
                      const std::string& asset_id,
                      const std::string& description,
                      const std::string& amount)
{
    auto cmd = cmdGenerator.generateTransferAsset(account_id, dest_account_id, asset_id, description, amount);
    addCommand(*cmd);
    return *this;
}

Tx& Tx::compareAndSetAccountDetail(const std::string& account_id,
                                   const std::string& key,
                                   const std::string& value,
                                   const std::optional<std::string>& old_value,
                                   bool check_empty)
{
    auto cmd = cmdGenerator.generateCompareAndSetAccountDetail(account_id, key, value, old_value, check_empty);
    addCommand(*cmd);
    return *this;
}

const iroha::protocol::Transaction Tx::signAndAddSignature()
{
    auto signature = iroha::sign(iroha::hash(protobufTransaction).to_string(),
                                 keypair.pubkey,
                                 keypair.privkey);

    auto proto_signature = protobufTransaction.add_signatures();
    proto_signature->set_public_key(keypair.pubkey.to_hexstring());
    proto_signature->set_signature(signature.to_hexstring());
    return protobufTransaction;
}

}

#include "CommandGenerator.hpp"
#include "model/converters/json_transaction_factory.hpp"


namespace iroha_lib
{

std::shared_ptr<iroha::protocol::Command> CommandGenerator::generateAddAssetQuantity(const std::string &asset_id,
                                                                                     const std::string &amount)
{
    iroha::protocol::AddAssetQuantity addAssetQuantity;
    addAssetQuantity.set_asset_id(asset_id);
    addAssetQuantity.set_amount(amount);

    auto cmd = iroha::protocol::Command();
    cmd.set_allocated_add_asset_quantity(new iroha::protocol::AddAssetQuantity(addAssetQuantity));
    return generateCommand<iroha::protocol::Command>(cmd);
}

std::shared_ptr<iroha::protocol::Command> CommandGenerator::generateAddPeer(const std::string& address,
                                                                            const std::string& pubkey,
                                                                            const std::optional<std::string>& tls_certificate,
                                                                            bool syncing_peer)
{
    iroha::protocol::AddPeer pb_add_peer;
    auto peer = pb_add_peer.mutable_peer();

    iroha::protocol::Peer res;
    res.set_address(address);
    res.set_peer_key(iroha::hexstringToArray<iroha::pubkey_t::size()>(pubkey).value().to_hexstring());

    if (tls_certificate.has_value())
        res.set_tls_certificate(*std::move(tls_certificate));
    res.set_syncing_peer(syncing_peer);

    peer->CopyFrom(res);

    auto cmd = iroha::protocol::Command();
    cmd.set_allocated_add_peer(new iroha::protocol::AddPeer(pb_add_peer));
    return generateCommand<iroha::protocol::Command>(cmd);
}

std::shared_ptr<iroha::protocol::Command> CommandGenerator::generateAddSignatory(const std::string& account_id,
                                                                                 const std::string& pubkey)
{
    iroha::protocol::AddSignatory pb_add_signatory;
    pb_add_signatory.set_account_id(account_id);
    pb_add_signatory.set_public_key(iroha::hexstringToArray<iroha::pubkey_t::size()>(pubkey).value().to_hexstring());
    // pb_add_signatory.set_public_key(std::move(pubkey)); // alternatywa-ok?

    auto cmd = iroha::protocol::Command();
    cmd.set_allocated_add_signatory(new iroha::protocol::AddSignatory(pb_add_signatory));
    return generateCommand<iroha::protocol::Command>(cmd);
}

std::shared_ptr<iroha::protocol::Command> CommandGenerator::generateAppendRole(const std::string& account_id,
                                                                               const std::string& role_name)
{
    iroha::protocol::AppendRole append_role;
    append_role.set_account_id(account_id);
    append_role.set_role_name(role_name);

    auto cmd = iroha::protocol::Command();
    cmd.set_allocated_append_role(new iroha::protocol::AppendRole(append_role));
    return generateCommand<iroha::protocol::Command>(cmd);
}

std::shared_ptr<iroha::protocol::Command> CommandGenerator::generateCreateAccount(const std::string& account_name,
                                                                                  const std::string& domain_id,
                                                                                  const std::string& pubkey)
{
    iroha::protocol::CreateAccount pb_create_account;
    pb_create_account.set_account_name(account_name);
    pb_create_account.set_domain_id(domain_id);
    pb_create_account.set_public_key(iroha::hexstringToArray<iroha::pubkey_t::size()>(pubkey).value().to_hexstring());
    // pb_create_account.set_public_key(pubkey); // alternatywa-ok?

    auto cmd = iroha::protocol::Command();
    cmd.set_allocated_create_account(new iroha::protocol::CreateAccount(pb_create_account));
    return generateCommand<iroha::protocol::Command>(cmd);
}

std::shared_ptr<iroha::protocol::Command> CommandGenerator::generateCreateAsset(const std::string &asset_name,
                                                                                const std::string &domain_id,
                                                                                uint8_t precision)
{

    iroha::protocol::CreateAsset pb_create_asset;
    pb_create_asset.set_asset_name(asset_name);
    pb_create_asset.set_domain_id(domain_id);
    pb_create_asset.set_precision(precision);

    auto cmd = iroha::protocol::Command();
    cmd.set_allocated_create_asset(new iroha::protocol::CreateAsset(pb_create_asset));
    return generateCommand<iroha::protocol::Command>(cmd);
}

std::shared_ptr<iroha::protocol::Command> CommandGenerator::generateCreateDomain(const std::string &domain_id,
                                                                                 const std::string &default_role)
{

    iroha::protocol::CreateDomain pb_create_domain;
    pb_create_domain.set_domain_id(domain_id);
    pb_create_domain.set_default_role(default_role);

    auto cmdCreateDomain = iroha::protocol::Command();
    cmdCreateDomain.set_allocated_create_domain(new iroha::protocol::CreateDomain(pb_create_domain));
    return generateCommand<iroha::protocol::Command>(cmdCreateDomain);
}

std::shared_ptr<iroha::protocol::Command> CommandGenerator::generateCreateRole(const std::string& roleName,
                                                                               const std::unordered_set<iroha::protocol::RolePermission>& permissions)
{
    iroha::protocol::CreateRole createRole;
    createRole.set_role_name(roleName);
    std::for_each(permissions.begin(),
                  permissions.end(),
                  [&createRole](auto permission)
    {
        createRole.add_permissions(permission);
    });

    auto cmd = iroha::protocol::Command();
    cmd.set_allocated_create_role(new iroha::protocol::CreateRole(createRole));
    return generateCommand<iroha::protocol::Command>(cmd);
}

std::shared_ptr<iroha::protocol::Command> CommandGenerator::generateDetachRole(const std::string& account_id,
                                                                               const std::string& role_name)
{
    iroha::protocol::DetachRole detach_role;
    detach_role.set_account_id(account_id);
    detach_role.set_role_name(role_name);

    auto cmd = iroha::protocol::Command();
    cmd.set_allocated_detach_role(new iroha::protocol::DetachRole(detach_role));
    return generateCommand<iroha::protocol::Command>(cmd);
}

std::shared_ptr<iroha::protocol::Command> CommandGenerator::generateGrantPermission(const std::string& account_id,
                                                                                    const iroha::protocol::GrantablePermission permission)
{
    iroha::protocol::GrantPermission grantPermission;
    grantPermission.set_account_id(account_id);
    grantPermission.set_permission(permission);

    auto cmd = iroha::protocol::Command();
    cmd.set_allocated_grant_permission(new iroha::protocol::GrantPermission(grantPermission));
    return generateCommand<iroha::protocol::Command>(cmd);
}

std::shared_ptr<iroha::protocol::Command> CommandGenerator::generateRemovePeer(const std::string& pubkey)
{
    iroha::protocol::RemovePeer removePeer;
    removePeer.set_public_key(pubkey);

    auto cmd = iroha::protocol::Command();
    cmd.set_allocated_remove_peer(new iroha::protocol::RemovePeer(removePeer));
    return generateCommand<iroha::protocol::Command>(cmd);
}

std::shared_ptr<iroha::protocol::Command> CommandGenerator::generateRemoveSignatory(const std::string& account_id, const std::string& pubkey)
{
    iroha::protocol::RemoveSignatory removeSignatory;
    removeSignatory.set_account_id(account_id);
    removeSignatory.set_public_key(iroha::hexstringToArray<iroha::pubkey_t::size()>(pubkey).value().to_hexstring());
    // pb_remove_signatory.set_public_key(keypair_.pubkey.to_hexstring()); // alternatywa-ok?

    auto cmd = iroha::protocol::Command();
    cmd.set_allocated_remove_signatory(new iroha::protocol::RemoveSignatory(removeSignatory));
    return generateCommand<iroha::protocol::Command>(cmd);

}

std::shared_ptr<iroha::protocol::Command> CommandGenerator::generateRevokePermission(const std::string& account_id,
                                                                                     const iroha::protocol::GrantablePermission permission)
{
    iroha::protocol::RevokePermission revokdePermission;
    revokdePermission.set_account_id(account_id);
    revokdePermission.set_permission(permission);

    auto cmd = iroha::protocol::Command();
    cmd.set_allocated_revoke_permission(new iroha::protocol::RevokePermission(revokdePermission));
    return generateCommand<iroha::protocol::Command>(cmd);
}

std::shared_ptr<iroha::protocol::Command> CommandGenerator::generateSetAccountDetail(const std::string& account_id,
                                                                                     const std::string& key,
                                                                                     const std::string& value)
{
    iroha::protocol::SetAccountDetail accountDetails;
    accountDetails.set_account_id(account_id);
    accountDetails.set_key(key);
    accountDetails.set_value(value);

    auto cmd = iroha::protocol::Command();
    cmd.set_allocated_set_account_detail(new iroha::protocol::SetAccountDetail(accountDetails));
    return generateCommand<iroha::protocol::Command>(cmd);
}

std::shared_ptr<iroha::protocol::Command> CommandGenerator::generateSetAccountQuorum(const std::string& account_id, uint32_t quorum)
{
    iroha::protocol::SetAccountQuorum setAccountQuorum;
    setAccountQuorum.set_account_id(account_id);
    setAccountQuorum.set_quorum(quorum);

    auto cmd = iroha::protocol::Command();
    cmd.set_allocated_set_account_quorum(new iroha::protocol::SetAccountQuorum(setAccountQuorum));
    return generateCommand<iroha::protocol::Command>(cmd);
}

std::shared_ptr<iroha::protocol::Command> CommandGenerator::generateSubtractAssetQuantity(const std::string& asset_id,
                                                                                          const std::string& amount)
{
    iroha::protocol::SubtractAssetQuantity subtractAssetQuantity;
    subtractAssetQuantity.set_asset_id(asset_id);
    subtractAssetQuantity.set_amount(amount);

    auto cmd = iroha::protocol::Command();
    cmd.set_allocated_subtract_asset_quantity(new iroha::protocol::SubtractAssetQuantity(subtractAssetQuantity));
    return generateCommand<iroha::protocol::Command>(cmd);
}

std::shared_ptr<iroha::protocol::Command> CommandGenerator::generateTransferAsset(const std::string& account_id,
                                                                                  const std::string& dest_account_id,
                                                                                  const std::string& asset_id,
                                                                                  const std::string& description,
                                                                                  const std::string& amount)
{
    iroha::protocol::TransferAsset transferAsset;
    transferAsset.set_src_account_id(account_id);
    transferAsset.set_dest_account_id(dest_account_id);
    transferAsset.set_asset_id(asset_id);
    transferAsset.set_description(description);
    transferAsset.set_amount(amount);

    auto cmd = iroha::protocol::Command();
    cmd.set_allocated_transfer_asset(new iroha::protocol::TransferAsset(transferAsset));
    return generateCommand<iroha::protocol::Command>(cmd);
}

std::shared_ptr<iroha::protocol::Command> CommandGenerator::generateCompareAndSetAccountDetail(const std::string& account_id,
                                                                                               const std::string& key,
                                                                                               const std::string& value,
                                                                                               const std::optional<std::string>& old_value,
                                                                                               bool check_empty)
{
    iroha::protocol::CompareAndSetAccountDetail compareAndSetAccountDetail;
    compareAndSetAccountDetail.set_account_id(account_id);
    compareAndSetAccountDetail.set_key(key);
    compareAndSetAccountDetail.set_value(value);
    if (old_value.has_value())
        compareAndSetAccountDetail.set_old_value(*std::move(old_value));
    compareAndSetAccountDetail.set_check_empty(check_empty);

    auto cmd = iroha::protocol::Command();
    cmd.set_allocated_compare_and_set_account_detail(new iroha::protocol::CompareAndSetAccountDetail(compareAndSetAccountDetail));
    return generateCommand<iroha::protocol::Command>(cmd);
}

}

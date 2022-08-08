#include "services/Tx.hpp"

#include <fstream>
#include <utility>

#include "GrpcClient.hpp"
#include "GrpcResponseHandler.hpp"
#include "model/converters/json_transaction_factory.hpp"
#include "model/converters/pb_common.hpp"
#include "model/permissions.hpp"
#include "cryptography/ed25519_sha3_impl/internal/ed25519_impl.hpp"
#include <boost/assign/list_inserter.hpp>

using namespace iroha::model;
using namespace shared_model::permissions;


Tx::Tx(const std::string& creator_account,
       const std::string& server_ip,
       int server_port,
       logger::LoggerManagerTreePtr response_handler_log_manager,
       logger::LoggerPtr pb_qry_factory_log,
       const iroha::keypair_t& keypair,
       uint64_t created_time,
       uint32_t quorum)
    : Request(server_ip, server_port),
      creator_(creator_account),
      response_handler_log_manager_(std::move(response_handler_log_manager)),
      pb_qry_factory_log_(std::move(pb_qry_factory_log)),
      keypair_(keypair)
{
    auto pl = pbtx.mutable_payload()->mutable_reduced_payload();
    pl->set_created_time(created_time);
    pl->set_creator_account_id(creator_);
    pl->set_quorum(quorum);

    populateRoleMap();
    populateGrantMap();
}

void Tx::populateRoleMap()
{
    boost::assign::insert(pb_role_map_)
            // Can append role
            (iroha::protocol::RolePermission::can_append_role, can_append_role)
            // Can create role
            (iroha::protocol::RolePermission::can_create_role, can_create_role)
            // Can append role
            (iroha::protocol::RolePermission::can_detach_role, can_detach_role)
            // Can add asset quantity
            (iroha::protocol::RolePermission::can_add_asset_qty, can_add_asset_qty)
            // Can subtract asset quantity
            (iroha::protocol::RolePermission::can_subtract_asset_qty,
             can_subtract_asset_qty)
            // Can add peer
            (iroha::protocol::RolePermission::can_add_peer, can_add_peer)
            // Can add signatory
            (iroha::protocol::RolePermission::can_add_signatory, can_add_signatory)
            // Can remove signatory
            (iroha::protocol::RolePermission::can_remove_signatory,
             can_remove_signatory)
            // Can set quorum
            (iroha::protocol::RolePermission::can_set_quorum, can_set_quorum)
            // Can create account
            (iroha::protocol::RolePermission::can_create_account, can_create_account)
            // Can set detail
            (iroha::protocol::RolePermission::can_set_detail, can_set_detail)
            // Can create asset
            (iroha::protocol::RolePermission::can_create_asset, can_create_asset)
            // Can transfer
            (iroha::protocol::RolePermission::can_transfer, can_transfer)
            // Can receive
            (iroha::protocol::RolePermission::can_receive, can_receive)
            // Can create domain
            (iroha::protocol::RolePermission::can_create_domain, can_create_domain)
            // Can read assets
            (iroha::protocol::RolePermission::can_read_assets, can_read_assets)
            // Can get roles
            (iroha::protocol::RolePermission::can_get_roles, can_get_roles)
            // Can get my account
            (iroha::protocol::RolePermission::can_get_my_account, can_get_my_account)
            // Can get all accounts
            (iroha::protocol::RolePermission::can_get_all_accounts,
             can_get_all_accounts)
            // Can get domain accounts
            (iroha::protocol::RolePermission::can_get_domain_accounts,
             can_get_domain_accounts)
            // Can get my signatories
            (iroha::protocol::RolePermission::can_get_my_signatories,
             can_get_my_signatories)
            // Can get all signatories
            (iroha::protocol::RolePermission::can_get_all_signatories,
             can_get_all_signatories)
            // Can get my engine receipts.
            (iroha::protocol::RolePermission::can_get_my_engine_receipts,
             can_get_my_engine_receipts)
            // Can get domain engine receipts.
            (iroha::protocol::RolePermission::can_get_domain_engine_receipts,
             can_get_domain_engine_receipts)
            // Can get all engine receipts.
            (iroha::protocol::RolePermission::can_get_all_engine_receipts,
             can_get_all_engine_receipts)
            // Can get domain signatories
            (iroha::protocol::RolePermission::can_get_domain_signatories,
             can_get_domain_signatories)
            // Can get my account assets
            (iroha::protocol::RolePermission::can_get_my_acc_ast, can_get_my_acc_ast)
            // Can get all account assets
            (iroha::protocol::RolePermission::can_get_all_acc_ast, can_get_all_acc_ast)
            // Can get domain account assets
            (iroha::protocol::RolePermission::can_get_domain_acc_ast,
             can_get_domain_acc_ast)
            // Can get my account detail
            (iroha::protocol::RolePermission::can_get_my_acc_detail,
             can_get_my_acc_detail)
            // Can get all account detail
            (iroha::protocol::RolePermission::can_get_all_acc_detail,
             can_get_all_acc_detail)
            // Can get domain account detail
            (iroha::protocol::RolePermission::can_get_domain_acc_detail,
             can_get_domain_acc_detail)
            // Can get my account transactions
            (iroha::protocol::RolePermission::can_get_my_acc_txs, can_get_my_acc_txs)
            // Can get all account transactions
            (iroha::protocol::RolePermission::can_get_all_acc_txs, can_get_all_acc_txs)
            // Can get domain account transactions
            (iroha::protocol::RolePermission::can_get_domain_acc_txs,
             can_get_domain_acc_txs)
            // Can get my account assets transactions
            (iroha::protocol::RolePermission::can_get_my_acc_ast_txs,
             can_get_my_acc_ast_txs)
            // Can get all account asset transactions
            (iroha::protocol::RolePermission::can_get_all_acc_ast_txs,
             can_get_all_acc_ast_txs)
            // Can get domain account asset transactions
            (iroha::protocol::RolePermission::can_get_domain_acc_ast_txs,
             can_get_domain_acc_ast_txs)
            // Can get my transactions
            (iroha::protocol::RolePermission::can_get_my_txs, can_get_my_txs)
            // Can get all transactions
            (iroha::protocol::RolePermission::can_get_all_txs, can_get_all_txs)

            // Can grant set quorum
            (iroha::protocol::RolePermission::can_grant_can_set_my_quorum,
             can_grant + can_set_my_quorum)
            // Can grant add signatory
            (iroha::protocol::RolePermission::can_grant_can_add_my_signatory,
             can_grant + can_add_my_signatory)
            // Can grant remove signatory
            (iroha::protocol::RolePermission::can_grant_can_remove_my_signatory,
             can_grant + can_remove_my_signatory)
            // Can grant can_transfer
            (iroha::protocol::RolePermission::can_grant_can_transfer_my_assets,
             can_grant + can_transfer_my_assets)
            // Can write details to other accounts
            (iroha::protocol::RolePermission::can_grant_can_set_my_account_detail,
             can_grant + can_set_my_account_detail)

            // Can get blocks
            (iroha::protocol::RolePermission::can_get_blocks, can_get_blocks);
}

void Tx::populateGrantMap()
{
    boost::assign::insert(pb_grant_map_)
            // Can add my signatory
            (iroha::protocol::GrantablePermission::can_add_my_signatory,
             can_add_signatory)
            // Can remove my signatory
            (iroha::protocol::GrantablePermission::can_remove_my_signatory,
             can_remove_signatory)
            // Can set my quorum
            (iroha::protocol::GrantablePermission::can_set_my_quorum, can_set_quorum)
            // Can write details to other accounts
            (iroha::protocol::GrantablePermission::can_set_my_account_detail,
             can_set_detail)
            // Can transfer my assets
            (iroha::protocol::GrantablePermission::can_transfer_my_assets,
             can_transfer);
}

void Tx::addCommand(const iroha::protocol::Command& command)
{
    auto pl = pbtx.mutable_payload()->mutable_reduced_payload();
    auto cmd = pl->add_commands();
    new (cmd)
            iroha::protocol::Command(command);
}

Tx& Tx::createRole(const std::string& role_name, const std::set<std::string>& permissions)
{
    iroha::protocol::CreateRole cmd;
    cmd.set_role_name(role_name);
    std::for_each(permissions.begin(),
                  permissions.end(),
                  [&cmd, this](auto perm) {
        auto perm_name = this->pb_role_map_.right.at(perm); // TODO sprawdzic czy ok
        cmd.add_permissions(perm_name);
    });

    auto cmdCreateRole = iroha::protocol::Command();
    cmdCreateRole.set_allocated_create_role(new iroha::protocol::CreateRole(cmd));

    addCommand(cmdCreateRole);
    return *this;
}

Tx& Tx::appendRole(const std::string& account_id, const std::string& role_name)
{
    iroha::protocol::AppendRole append_role;
    append_role.set_account_id(account_id);
    append_role.set_role_name(role_name);

    auto cmdAppendRole = iroha::protocol::Command();
    cmdAppendRole.set_allocated_append_role(new iroha::protocol::AppendRole(append_role));

    addCommand(cmdAppendRole);
    return *this;
}

Tx& Tx::detachRole(const std::string& account_id, const std::string& role_name)
{
    iroha::protocol::DetachRole cmd;
    cmd.set_account_id(account_id);
    cmd.set_role_name(role_name);

    auto cmdDetachRole = iroha::protocol::Command();
    cmdDetachRole.set_allocated_detach_role(new iroha::protocol::DetachRole(cmd));

    addCommand(cmdDetachRole);
    return *this;
}

Tx& Tx::grantPermission(const std::string& account_id, const std::string& permission_name)
{
    iroha::protocol::GrantPermission cmd;
    cmd.set_account_id(account_id);
    auto perm = pb_grant_map_.right.at(permission_name); // TODO ok?
    cmd.set_permission(perm);

    auto cmdGrantPermission = iroha::protocol::Command();
    cmdGrantPermission.set_allocated_grant_permission(new iroha::protocol::GrantPermission(cmd));

    addCommand(cmdGrantPermission);
    return *this;
}

Tx& Tx::revokePermission(const std::string& account_id, const std::string& permission_name)
{
    iroha::protocol::RevokePermission cmd;
    cmd.set_account_id(account_id);
    auto permission = pb_grant_map_.right.at(permission_name);  // TODO ok?
    cmd.set_permission(permission);

    auto cmdRevokePermission = iroha::protocol::Command();
    cmdRevokePermission.set_allocated_revoke_permission(new iroha::protocol::RevokePermission(cmd));

    addCommand(cmdRevokePermission);
    return *this;
}

Tx& Tx::addAssetQuantity(const std::string& asset_id, const std::string& amount)
{
    iroha::protocol::AddAssetQuantity pb_add_asset_quantity;
    pb_add_asset_quantity.set_asset_id(asset_id);
    pb_add_asset_quantity.set_amount(amount);

    auto cmdAddAssetQuantity = iroha::protocol::Command();
    cmdAddAssetQuantity.set_allocated_add_asset_quantity(new iroha::protocol::AddAssetQuantity(pb_add_asset_quantity));

    addCommand(cmdAddAssetQuantity);
    return *this;
}

Tx& Tx::subtractAssetQuantity(const std::string& asset_id, const std::string& amount)
{
    iroha::protocol::SubtractAssetQuantity pb_subtract_asset_quantity;
    pb_subtract_asset_quantity.set_asset_id(asset_id);
    pb_subtract_asset_quantity.set_amount(amount);

    auto cmdSubtractAssetQuantity = iroha::protocol::Command();
    cmdSubtractAssetQuantity.set_allocated_subtract_asset_quantity(new iroha::protocol::SubtractAssetQuantity(pb_subtract_asset_quantity));

    addCommand(cmdSubtractAssetQuantity);
    return *this;
}

Tx& Tx::addPeer(const std::string& address, const std::string& pubkey)
{
    iroha::protocol::AddPeer pb_add_peer;
    auto peer = pb_add_peer.mutable_peer();

    iroha::protocol::Peer res;
    res.set_address(address);
    res.set_peer_key(iroha::hexstringToArray<iroha::pubkey_t::size()>(pubkey).value().to_hexstring());

    peer->CopyFrom(res);

    auto cmdAddPeer = iroha::protocol::Command();
    cmdAddPeer.set_allocated_add_peer(new iroha::protocol::AddPeer(pb_add_peer));

    addCommand(cmdAddPeer);
    return *this;
}

Tx& Tx::addSignatory(const std::string& account_id, const std::string& pubkey)
{
    iroha::protocol::AddSignatory pb_add_signatory;
    pb_add_signatory.set_account_id(account_id);
    pb_add_signatory.set_public_key(iroha::hexstringToArray<iroha::pubkey_t::size()>(pubkey).value().to_hexstring());
    // pb_add_signatory.set_public_key(std::move(pubkey)); // alternatywa-ok?

    auto cmdAddSignatory = iroha::protocol::Command();
    cmdAddSignatory.set_allocated_add_signatory(new iroha::protocol::AddSignatory(pb_add_signatory));

    addCommand(cmdAddSignatory);
    return *this;
}

Tx& Tx::createAsset(const std::string& asset_name, const std::string& domain_id, uint32_t precision)
{
    iroha::protocol::CreateAsset pb_create_asset;
    pb_create_asset.set_asset_name(asset_name);
    pb_create_asset.set_domain_id(domain_id);
    pb_create_asset.set_precision(precision);

    auto cmdCreateAsset = iroha::protocol::Command();
    cmdCreateAsset.set_allocated_create_asset(new iroha::protocol::CreateAsset(pb_create_asset));

    //    auto pl = pbtx.mutable_payload()->mutable_reduced_payload();
    //    auto cmd = pl->add_commands();
    //    new (cmd)
    //        iroha::protocol::Command(cmdCreateAsset);
    addCommand(cmdCreateAsset);
    return *this;
}

Tx& Tx::createAccount(const std::string& account_id, const std::string& domain_id, const std::string& pubkey)
{
    iroha::protocol::CreateAccount pb_create_account;
    pb_create_account.set_account_name(account_id);
    pb_create_account.set_domain_id(domain_id);
    pb_create_account.set_public_key(iroha::hexstringToArray<iroha::pubkey_t::size()>(pubkey).value().to_hexstring());
    // pb_create_account.set_public_key(pubkey); // alternatywa-ok?

    auto cmdCreateAccount = iroha::protocol::Command();
    cmdCreateAccount.set_allocated_create_account(new iroha::protocol::CreateAccount(pb_create_account));

    addCommand(cmdCreateAccount);
    return *this;
}

Tx& Tx::setAccountDetail(const std::string& account_id, const std::string& key, const std::string& value)
{
    iroha::protocol::SetAccountDetail cmd;
    cmd.set_account_id(account_id);
    cmd.set_key(key);
    cmd.set_value(value);

    auto cmdSetAccountDetail = iroha::protocol::Command();
    cmdSetAccountDetail.set_allocated_set_account_detail(new iroha::protocol::SetAccountDetail(cmd));

    addCommand(cmdSetAccountDetail);
    return *this;
}

Tx& Tx::createDomain(const std::string& domain_id, const std::string& user_default_role)
{
    iroha::protocol::CreateDomain pb_create_domain;
    pb_create_domain.set_domain_id(domain_id);
    pb_create_domain.set_default_role(user_default_role);

    auto cmdCreateDomain = iroha::protocol::Command();
    cmdCreateDomain.set_allocated_create_domain(new iroha::protocol::CreateDomain(pb_create_domain));

    //    auto pl = pbtx.mutable_payload()->mutable_reduced_payload();
    //    auto cmd = pl->add_commands();
    //    new (cmd)
    //        iroha::protocol::Command(cmdCreateDomain);
    addCommand(cmdCreateDomain);
    return *this;
}

Tx& Tx::removeSignatory(const std::string& account_id, const std::string& pubkey)
{
    iroha::protocol::RemoveSignatory pb_remove_signatory;
    pb_remove_signatory.set_account_id(account_id);
    pb_remove_signatory.set_public_key(iroha::hexstringToArray<iroha::pubkey_t::size()>(pubkey).value().to_hexstring());
    // pb_remove_signatory.set_public_key(keypair_.pubkey.to_hexstring()); // alternatywa-ok?

    auto cmdRemoveSignatory = iroha::protocol::Command();
    cmdRemoveSignatory.set_allocated_remove_signatory(new iroha::protocol::RemoveSignatory(pb_remove_signatory));

    addCommand(cmdRemoveSignatory);
    return *this;
}

Tx& Tx::setAccountQuorum(const std::string& account_id,
                         uint32_t quorum)
{
    iroha::protocol::SetAccountQuorum pb_set_account_quorum;
    pb_set_account_quorum.set_account_id(account_id);
    pb_set_account_quorum.set_quorum(quorum);

    auto cmdSetAccountQuorum = iroha::protocol::Command();
    cmdSetAccountQuorum.set_allocated_set_account_quorum(new iroha::protocol::SetAccountQuorum(pb_set_account_quorum));

    addCommand(cmdSetAccountQuorum);
    return *this;
}

Tx& Tx::transferAsset(const std::string& account_id,
                      const std::string& dest_account_id,
                      const std::string& asset_id,
                      const std::string& description,
                      const std::string& amount)
{
    iroha::protocol::TransferAsset pb_transfer_asset;
    pb_transfer_asset.set_src_account_id(account_id);
    pb_transfer_asset.set_dest_account_id(dest_account_id);
    pb_transfer_asset.set_asset_id(asset_id);
    pb_transfer_asset.set_description(description);
    pb_transfer_asset.set_amount(amount);

    auto cmdTransferAsset = iroha::protocol::Command();
    cmdTransferAsset.set_allocated_transfer_asset(new iroha::protocol::TransferAsset(pb_transfer_asset));

    addCommand(cmdTransferAsset);
    return *this;
}

Tx& Tx::signAndAddSignature()
{
    auto signature = iroha::sign(iroha::hash(pbtx).to_string(),
                                 keypair_.pubkey,
                                 keypair_.privkey);

    auto proto_signature = pbtx.add_signatures();
    proto_signature->set_public_key(keypair_.pubkey.to_hexstring());
    proto_signature->set_signature(signature.to_hexstring());

    return *this;
}

bool Tx::send()
{
    GrpcResponseHandler response_handler(response_handler_log_manager_);
    response_handler.handle(GrpcClient(getServerIp(), getServerPort(), pb_qry_factory_log_).sendTx(pbtx));
    printTransactionHash(pbtx);
    return true;
}

void Tx::printTransactionHash(iroha::protocol::Transaction& tx) const
{
    const std::string tx_hash = iroha::hash(tx).to_hexstring();
    std::cout << "Transaction was accepted for processing." << std::endl;
    std::cout << "Its hash is " << tx_hash << std::endl;
}

#include "GrpcClient.hpp"
#include "cryptography/ed25519_sha3_impl/internal/ed25519_impl.hpp"
#include "GrpcResponseHandler.hpp"
#include "services/Query.hpp"
#include "model/converters/pb_common.hpp"


namespace IROHA_CPP
{

Query::Query(const std::string& account_name, uint64_t query_counter, const iroha::keypair_t& keypair)
    : creator_(account_name),
      counter_(query_counter),
      keypair_(keypair)
{}

void Query::populateQueryMetaData(iroha::protocol::Query& pb_query)
{
    auto* meta = pb_query.mutable_payload()->mutable_meta();
    meta->set_created_time(local_time_);
    meta->set_creator_account_id(creator_);
    meta->set_query_counter(counter_);
}

Query& Query::getAccount(const std::string& account_id)
{
    iroha::protocol::Query pb_query;
    auto pl = pb_query.mutable_payload();
    populateQueryMetaData(pb_query);

    auto pb_query_mut = pl->mutable_get_account();
    pb_query_mut->set_account_id(account_id);

    queryProto = pb_query;
    return *this;
}

Query& Query::getAccountAssets(const std::string& account_id)
{
    iroha::protocol::Query pb_query;
    populateQueryMetaData(pb_query);

    auto pb_query_mut = pb_query.mutable_payload()->mutable_get_account_assets();
    pb_query_mut->set_account_id(account_id);

    queryProto = pb_query;
    return *this;
}

Query& Query::getAccountDetail(const std::string& account_id)
{
    iroha::protocol::Query pb_query;
    populateQueryMetaData(pb_query);

    auto pb_query_mut = pb_query.mutable_payload()->mutable_get_account_detail();
    pb_query_mut->set_account_id(account_id);

    queryProto = pb_query;
    return *this;
}

Query& Query::getAccountTransactions(const std::string& account_id)
{
    iroha::protocol::Query pb_query;
    populateQueryMetaData(pb_query);

    auto pb_query_mut = pb_query.mutable_payload()->mutable_get_account_transactions();
    pb_query_mut->set_account_id(account_id);

    queryProto = pb_query;
    return *this;
}

Query& Query::getAccountAssetTransactions(const std::string& account_id, const std::string& asset_id)
{
    iroha::protocol::Query pb_query;
    populateQueryMetaData(pb_query);

    auto pb_query_mut = pb_query.mutable_payload()->mutable_get_account_asset_transactions();
    pb_query_mut->set_account_id(account_id);
    pb_query_mut->set_asset_id(asset_id);

    queryProto = pb_query;
    return *this;
}

Query& Query::getTransactions(const std::vector<std::string>& tx_hashes)
{
    iroha::protocol::Query pb_query;
    populateQueryMetaData(pb_query);

    auto pb_query_mut = pb_query.mutable_payload()->mutable_get_transactions();
    std::for_each(tx_hashes.begin(),
                  tx_hashes.end(),
                  [&pb_query_mut](auto tx_hash)
    {
        auto adder = pb_query_mut->add_tx_hashes();
        //        *adder = tx_hash.to_hexstring();    // TODO string do hexstring()
    });

    queryProto = pb_query;
    return *this;
}

Query& Query::getSignatories(const std::string& account_id)
{
    iroha::protocol::Query pb_query;
    populateQueryMetaData(pb_query);

    auto pb_query_mut = pb_query.mutable_payload()->mutable_get_signatories();
    pb_query_mut->set_account_id(account_id);

    queryProto = pb_query;
    return *this;
}

Query& Query::getAssetInfo(const std::string& asset_id)
{
    iroha::protocol::Query pb_query;
    populateQueryMetaData(pb_query);

    auto pb_query_mut = pb_query.mutable_payload()->mutable_get_asset_info();
    pb_query_mut->set_asset_id(asset_id);

    queryProto = pb_query;
    return *this;
}

Query& Query::getRoles()
{
    iroha::protocol::Query pb_query;
    pb_query.mutable_payload()->mutable_get_roles();
    populateQueryMetaData(pb_query);

    queryProto = pb_query;
    return *this;
}

Query& Query::getRolePermissions(const std::string& role_id)
{
    iroha::protocol::Query pb_query;
    populateQueryMetaData(pb_query);

    auto pb_query_mut = pb_query.mutable_payload()->mutable_get_role_permissions();
    pb_query_mut->set_role_id(role_id);

    queryProto = pb_query;
    return *this;
}

iroha::protocol::Query& Query::signAndAddSignature()
{
    auto signature = iroha::sign(iroha::hash(queryProto).to_string(),
                                 keypair_.pubkey,
                                 keypair_.privkey);

    auto sig = queryProto.mutable_signature();
    sig->set_signature(signature.to_hexstring());
    sig->set_public_key(keypair_.pubkey.to_hexstring());
    return queryProto;
}

}

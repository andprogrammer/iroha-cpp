#include "GrpcClient.hpp"
#include "cryptography/ed25519_sha3_impl/internal/ed25519_impl.hpp"
#include "Query.hpp"
#include "model/converters/pb_common.hpp"


namespace iroha_lib
{

Query::Query(const std::string& account_id,
             uint64_t counter,
             const iroha::keypair_t& keypair,
             const uint64_t createdTime)
    : accountId(account_id),
      counter(counter),
      createdTime(createdTime),
      keypair(keypair)
{}

Query& Query::getAccount(const std::string& account_id)
{
    protobufQuery = *queryGenerator.generateGetAccount(account_id, counter, createdTime);
    return *this;
}

Query& Query::getAccountAssets(const std::string& account_id)
{
    protobufQuery = *queryGenerator.generateGetAccountAssets(account_id, counter, createdTime);
    return *this;
}

Query& Query::getAccountDetail(const std::string& account_id)
{
    protobufQuery = *queryGenerator.generateGetAccountDetail(account_id, counter, createdTime);
    return *this;
}

Query& Query::getAccountTransactions(const std::string& account_id)
{
    protobufQuery = *queryGenerator.generateGetAccountTransactions(account_id, counter, createdTime);
    return *this;
}

Query& Query::getAccountAssetTransactions(const std::string& account_id, const std::string& asset_id)
{
    protobufQuery = *queryGenerator.generateGetAccountAssetTransactions(account_id, counter, createdTime, asset_id);
    return *this;
}

Query& Query::getTransactions(const std::string& account_id, const std::vector<std::string>& tx_hashes)
{
    protobufQuery = *queryGenerator.generateGetTransactions(account_id, counter, createdTime, tx_hashes);
    return *this;
}

Query& Query::getSignatories(const std::string& account_id)
{
    protobufQuery = *queryGenerator.generateGetSignatories(account_id, counter, createdTime);
    return *this;
}

Query& Query::getAssetInfo(const std::string& account_id, const std::string& asset_id)
{
    protobufQuery = *queryGenerator.generateGetAssetInfo(account_id, counter, createdTime, asset_id);
    return *this;
}

Query& Query::getRoles(const std::string& account_id)
{
    protobufQuery = *queryGenerator.generateGetRoles(account_id, counter, createdTime);
    return *this;
}

Query& Query::getRolePermissions(const std::string& account_id, const std::string& role_id)
{
    protobufQuery = *queryGenerator.generateGetRolePermissions(account_id, counter, createdTime, role_id);
    return *this;
}

const iroha::protocol::Query Query::signAndAddSignature()
{
    auto signature = iroha::sign(iroha::hash(protobufQuery).to_string(),
                                 keypair.pubkey,
                                 keypair.privkey);

    auto sig = protobufQuery.mutable_signature();
    sig->set_signature(signature.to_hexstring());
    sig->set_public_key(keypair.pubkey.to_hexstring());
    return protobufQuery;
}

}

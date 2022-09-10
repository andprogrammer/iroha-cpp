#include "QueryGenerator.hpp"


namespace iroha_lib
{

std::shared_ptr<iroha::protocol::Query> QueryGenerator::generateQuery(const std::string& accountId,
                                                                      uint64_t counter,
                                                                      const uint64_t createdTime)
{
    auto query = std::make_shared<iroha::protocol::Query>();
    auto* payload = query->mutable_payload()->mutable_meta();
    payload->set_creator_account_id(accountId);
    payload->set_query_counter(counter);
    payload->set_created_time(createdTime);
    return query;
}

std::shared_ptr<iroha::protocol::Query> QueryGenerator::generateGetAccount(const std::string& accountId,
                                                                           uint64_t counter,
                                                                           const uint64_t createdTime)
{
    auto query = generateQuery(accountId, counter, createdTime);
    auto queryPayload = query->mutable_payload();
    auto mutablePayload = queryPayload->mutable_get_account();
    mutablePayload->set_account_id(accountId);
    return query;
}

std::shared_ptr<iroha::protocol::Query> QueryGenerator::generateGetAccountAssets(const std::string& accountId,
                                                                                 uint64_t counter,
                                                                                 const uint64_t createdTime)
{
    auto query = generateQuery(accountId, counter, createdTime);
    auto queryPayload = query->mutable_payload()->mutable_get_account_assets();
    queryPayload->set_account_id(accountId);
    return query;
}

std::shared_ptr<iroha::protocol::Query> QueryGenerator::generateGetAccountDetail(const std::string& accountId,
                                                                                 uint64_t counter,
                                                                                 const uint64_t createdTime)
{
    auto query = generateQuery(accountId, counter, createdTime);
    auto queryPayload = query->mutable_payload()->mutable_get_account_detail();
    queryPayload->set_account_id(accountId);
    return query;
}

std::shared_ptr<iroha::protocol::Query> QueryGenerator::generateGetAccountTransactions(const std::string& accountId,
                                                                                       uint64_t counter,
                                                                                       const uint64_t createdTime)
{
    auto query = generateQuery(accountId, counter, createdTime);
    auto queryPayload = query->mutable_payload()->mutable_get_account_transactions();
    // TODO
    //    https://iroha.readthedocs.io/en/develop/develop/api/queries.html#result-pagination
//    query->mutable_payload()->mutable_get_account_transactions()->mutable_pagination_meta()->set_allocated_first_tx_hash()
    queryPayload->set_account_id(accountId);
    return query;
}

std::shared_ptr<iroha::protocol::Query> QueryGenerator::generateGetAccountAssetTransactions(const std::string& accountId,
                                                                                            uint64_t counter,
                                                                                            const uint64_t createdTime,
                                                                                            const std::string& assetId)
{
    auto query = generateQuery(accountId, counter, createdTime);
    auto queryPayload = query->mutable_payload()->mutable_get_account_asset_transactions();
    queryPayload->set_account_id(accountId);
    queryPayload->set_asset_id(assetId);
    return query;
}

std::shared_ptr<iroha::protocol::Query> QueryGenerator::generateGetTransactions(const std::string& accountId,
                                                                                uint64_t counter,
                                                                                const uint64_t createdTime,
                                                                                const std::vector<std::string>& transactionHashes)
{
    auto query = generateQuery(accountId, counter, createdTime);
    auto queryPayload = query->mutable_payload()->mutable_get_transactions();
    std::for_each(transactionHashes.begin(),
                  transactionHashes.end(),
                  [&queryPayload](auto tx_hash)
    {
        auto adder = queryPayload->add_tx_hashes();
//        *adder = tx_hash.to_hexstring();    // TODO
    });
    return query;
}

std::shared_ptr<iroha::protocol::Query> QueryGenerator::generateGetSignatories(const std::string& accountId,
                                                                               uint64_t counter,
                                                                               const uint64_t createdTime)
{
    auto query = generateQuery(accountId, counter, createdTime);
    auto queryPayload = query->mutable_payload()->mutable_get_signatories();
    queryPayload->set_account_id(accountId);
    return query;
}

std::shared_ptr<iroha::protocol::Query> QueryGenerator::generateGetAssetInfo(const std::string& accountId,
                                                                             uint64_t counter,
                                                                             const uint64_t createdTime,
                                                                             const std::string& assetId)
{
    auto query = generateQuery(accountId, counter, createdTime);
    auto queryPayload = query->mutable_payload()->mutable_get_asset_info();
    queryPayload->set_asset_id(assetId);
    return query;
}

std::shared_ptr<iroha::protocol::Query> QueryGenerator::generateGetRoles(const std::string& accountId,
                                                                         uint64_t counter,
                                                                         const uint64_t createdTime)
{
    auto query = generateQuery(accountId, counter, createdTime);
    query->mutable_payload()->mutable_get_roles();
    return query;
}

std::shared_ptr<iroha::protocol::Query> QueryGenerator::generateGetRolePermissions(const std::string& accountId,
                                                                                   uint64_t counter,
                                                                                   const uint64_t createdTime,
                                                                                   const std::string& roleId)
{
    auto query = generateQuery(accountId, counter, createdTime);
    auto queryPayload = query->mutable_payload()->mutable_get_role_permissions();
    queryPayload->set_role_id(roleId);
    return query;
}

}

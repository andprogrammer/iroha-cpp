#ifndef QUERY_HPP
#define QUERY_HPP

#include "LibsAndClassDeclarations.h"

#include "crypto/keypair.hpp"
#include "queries.pb.h"
#include "generators/QueryGenerator.hpp"


namespace iroha_lib
{

class Query
{
    std::string accountId;
    uint64_t counter;
    uint64_t createdTime;
    iroha::protocol::Query protobufQuery;
    iroha::keypair_t keypair;
    QueryGenerator queryGenerator;

public:
    Query(const std::string& account_id,
          uint64_t counter,
          const iroha::keypair_t& keypair,
          const uint64_t createdTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());

    Query& getAccount(const std::string& account_id);
    Query& getAccountAssets(const std::string& account_id);
    Query& getAccountDetail(const std::string& account_id);
    Query& getAccountTransactions(const std::string& account_id);
    Query& getAccountAssetTransactions(const std::string& account_id, const std::string& asset_id);
    Query& getTransactions(const std::string& account_id, const std::vector<std::string>& tx_hashes);
    Query& getSignatories(const std::string& account_id);
    Query& getAssetInfo(const std::string& account_id, const std::string& asset_id);
    Query& getRoles(const std::string& account_id);
    Query& getRolePermissions(const std::string& account_id, const std::string& role_id);

    const iroha::protocol::Query signAndAddSignature();
};

}

#endif

#ifndef QUERY_HPP
#define QUERY_HPP

#include "LibsAndClassDeclarations.h"

#include "crypto/keypair.hpp"
#include "queries.pb.h"


namespace IROHA_CPP
{

class Query
{
    std::string creator_;
    uint64_t counter_;
    uint64_t local_time_;
    iroha::protocol::Query queryProto;
    iroha::keypair_t keypair_;

public:
    Query(const std::string& account_name, uint64_t query_counter, const iroha::keypair_t& keypair);
    void populateQueryMetaData(iroha::protocol::Query& pb_query);
    Query& getAccount(const std::string& account_id);
    Query& getAccountAssets(const std::string& account_id);
    Query& getAccountDetail(const std::string& account_id);
    Query& getAccountTransactions(const std::string& account_id);
    Query& getAccountAssetTransactions(const std::string& account_id, const std::string& asset_id);
    Query& getTransactions(const std::vector<std::string>& tx_hashes);
    Query& getSignatories(const std::string& account_id);
    Query& getAssetInfo(const std::string& asset_id);
    Query& getRoles();
    Query& getRolePermissions(const std::string& role_id);
    iroha::protocol::Query& signAndAddSignature();
};

}

#endif

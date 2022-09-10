#ifndef QUERY_GENERATOR_HPP
#define QUERY_GENERATOR_HPP

#include "queries.pb.h"


namespace iroha_lib
{

class QueryGenerator
{
    std::shared_ptr<iroha::protocol::Query> generateQuery(const std::string& accountId,
                                                          uint64_t counter,
                                                          const uint64_t createdTime);

public:
    std::shared_ptr<iroha::protocol::Query> generateGetAccount(const std::string& accountId,
                                                               uint64_t counter,
                                                               const uint64_t createdTime);
    std::shared_ptr<iroha::protocol::Query> generateGetAccountAssets(const std::string& accountId,
                                                                     uint64_t counter,
                                                                     const uint64_t createdTime);
    std::shared_ptr<iroha::protocol::Query> generateGetAccountDetail(const std::string& accountId,
                                                                     uint64_t counter,
                                                                     const uint64_t createdTime);
    std::shared_ptr<iroha::protocol::Query> generateGetAccountTransactions(const std::string& accountId,
                                                                           uint64_t counter,
                                                                           const uint64_t createdTime);
    std::shared_ptr<iroha::protocol::Query> generateGetAccountAssetTransactions(const std::string& accountId,
                                                                                uint64_t counter,
                                                                                const uint64_t createdTime,
                                                                                const std::string& assetId);
    std::shared_ptr<iroha::protocol::Query> generateGetTransactions(const std::string& accountId,
                                                                    uint64_t counter,
                                                                    const uint64_t createdTime,
                                                                    const std::vector<std::string>& transactionHashes);
    std::shared_ptr<iroha::protocol::Query> generateGetSignatories(const std::string& accountId,
                                                                   uint64_t counter,
                                                                   const uint64_t createdTime);
    std::shared_ptr<iroha::protocol::Query> generateGetAssetInfo(const std::string& accountId,
                                                                 uint64_t counter,
                                                                 const uint64_t createdTime,
                                                                 const std::string& assetId);
    std::shared_ptr<iroha::protocol::Query> generateGetRoles(const std::string& accountId,
                                                             uint64_t counter,
                                                             const uint64_t createdTime);
    std::shared_ptr<iroha::protocol::Query> generateGetRolePermissions(const std::string& accountId,
                                                                       uint64_t counter,
                                                                       const uint64_t createdTime,
                                                                       const std::string& roleId);
};

}

#endif

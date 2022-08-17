#ifndef QUERY_RESPONSE_HANDLER_HPP
#define QUERY_RESPONSE_HANDLER_HPP

#include <map>
#include <memory>
#include <typeindex>
#include <unordered_map>

#include "logger/logger_fwd.hpp"
#include "qry_responses.pb.h"


namespace IROHA_CPP
{


struct EnumTypeHash
{
    template <typename T>
    std::size_t operator()(T t) const
    {
        return static_cast<std::size_t>(t);
    }
};


class QueryResponseHandler
{
public:
    explicit QueryResponseHandler(logger::LoggerPtr log);
    void handle(const iroha::protocol::QueryResponse &response);

private:
    void handleErrorResponse(const iroha::protocol::QueryResponse &response);
    void handleAccountResponse(const iroha::protocol::QueryResponse &response);
    void handleAccountAssetsResponse(const iroha::protocol::QueryResponse &response);
    void handleTransactionsResponse(const iroha::protocol::QueryResponse &response);
    void handleSignatoriesResponse(const iroha::protocol::QueryResponse &response);
    void handleRolesResponse(const iroha::protocol::QueryResponse &response);
    void handleRolePermissionsResponse(const iroha::protocol::QueryResponse &response);
    void handleAssetResponse(const iroha::protocol::QueryResponse &response);

    using Handler = void (QueryResponseHandler::*)(const iroha::protocol::QueryResponse &);
    using QueryResponseCode = iroha::protocol::QueryResponse::ResponseCase;
    using ErrorResponseCode = iroha::protocol::ErrorResponse::Reason;

    std::unordered_map<QueryResponseCode, Handler, EnumTypeHash> handler_map_;
    std::unordered_map<ErrorResponseCode, std::string, EnumTypeHash>
    error_handler_map_;

    logger::LoggerPtr log_;
};

}

#endif

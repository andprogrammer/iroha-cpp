#include "services/TxBatch.hpp"

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


TxBatch::TxBatch(const std::string& server_ip,
                 int server_port,
                 logger::LoggerPtr pb_qry_factory_log,
                 logger::LoggerManagerTreePtr response_handler_log_manager)
    : Request(server_ip, server_port, std::move(pb_qry_factory_log)),
      response_handler_log_manager_(std::move(response_handler_log_manager))
{}

void TxBatch::addTransaction(const iroha::protocol::Transaction& tx)
{
    transactions_.push_back(tx);
}

const std::string TxBatch::send()
{
    iroha::protocol::TxList tx_list;
    for (const auto& tx : transactions_)
        *tx_list.add_transactions() = tx;

    GrpcResponseHandler response_handler(response_handler_log_manager_);
    response_handler.handle(GrpcClient(getServerIp(), getServerPort(), pb_qry_factory_log_).sendTxList(tx_list));
    return "200";
}

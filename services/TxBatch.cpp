#include "services/TxBatch.hpp"

#include "model/converters/pb_common.hpp"


namespace IROHA_CPP
{

void TxBatch::addTransaction(const iroha::protocol::Transaction& tx)
{
    transactions_.push_back(tx);
}

const std::vector<std::string> TxBatch::getTxHashes() const
{
    std::vector<std::string> txHashes;
    for (const auto& tx: transactions_)
    {
        txHashes.push_back(iroha::hash(tx).to_hexstring());
    }
    return txHashes;
}

iroha::protocol::TxList TxBatch::getTxList() const
{
    iroha::protocol::TxList tx_list;
    for (const auto& tx: transactions_)
    {
        *tx_list.add_transactions() = tx;
    }
    return tx_list;
}

}

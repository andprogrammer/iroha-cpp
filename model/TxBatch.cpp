#include "TxBatch.hpp"

#include "model/converters/pb_common.hpp"


namespace iroha_lib
{

iroha::protocol::Transaction::Payload::BatchMeta::BatchType TxBatch::getBatchType(bool atomic) const
{
    const auto atomicBatchType = iroha::protocol::Transaction::Payload::BatchMeta::BatchType::Transaction_Payload_BatchMeta_BatchType_ATOMIC;
    const auto orderedBatchType = iroha::protocol::Transaction::Payload::BatchMeta::BatchType::Transaction_Payload_BatchMeta_BatchType_ORDERED;
    return atomic ? atomicBatchType
                  : orderedBatchType;
}

iroha::protocol::TxList TxBatch::batch(std::vector<iroha::protocol::Transaction>& transactions, bool atomic)
{
    iroha::protocol::TxList tx_list;

    if (atomic)
    {
        iroha::protocol::Transaction::Payload::BatchMeta meta;
        meta.set_type(getBatchType(atomic));

        for (auto& tx: transactions)
        {
            tx.payload().batch().New()->CopyFrom(meta);
            *tx_list.add_transactions() = tx;
        }
    }
    else
    {
        for (const auto& tx: transactions)
        {
            *tx_list.add_transactions() = tx;
        }
    }
    return tx_list;
}

}

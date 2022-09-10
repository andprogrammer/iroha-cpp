#ifndef TX_BATCH_HPP
#define TX_BATCH_HPP

#include "LibsAndClassDeclarations.h"

#include "transaction.pb.h"
#include <endpoint.pb.h>


namespace iroha_lib
{

class TxBatch
{
public:
    iroha::protocol::Transaction::Payload::BatchMeta::BatchType getBatchType(bool atomic) const;
    iroha::protocol::TxList batch(std::vector<iroha::protocol::Transaction>& transactions, bool atomic = true);
};

}

#endif

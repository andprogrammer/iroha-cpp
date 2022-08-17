#ifndef TX_BATCH_HPP
#define TX_BATCH_HPP

#include "LibsAndClassDeclarations.h"

#include "transaction.pb.h"
#include <endpoint.pb.h>


namespace IROHA_CPP
{

class TxBatch
{
    std::vector<iroha::protocol::Transaction> transactions_;

public:
    void addTransaction(const iroha::protocol::Transaction& tx);
    const std::vector<std::string> send();
    const std::vector<std::string> getTxHashes() const;
    iroha::protocol::TxList getTxList() const;
};

}

#endif

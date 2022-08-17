# C++ library for Hyperledger Iroha


This is a source repository for HL Iroha C++ library.

Currently, latest HL Iroha rc5 release (`hyperledger/iroha:latest` Docker image) is supported.


### Installation

```bash
mkdir build && cd build && cmake .. && make 
```


### Usage Example

### Configuration
```c++
    auto account_name = "admin@test";
    const auto key_path = "/home/laptop/qt-workspace/iroha/example";
    const auto peer_ip = "127.0.0.1";
    int torii_port = 50051;
    const auto quorum = 1;

    const auto user_default_role = "user";
```

#### Transaction

```c++
    IROHA_CPP::Tx tx(account_name, keypair);

    const auto tx_proto = tx.createDomain(domain_id, user_default_role)
            .createAsset(asset_name, domain_id, 0)
            .signAndAddSignature();

    GrpcResponseHandler response_handler(response_handler_log_manager);
    response_handler.handle(GrpcClient(peer_ip, torii_port, pb_qry_factory_log).sendTx(tx_proto));

    IROHA_CPP::Status status(tx_hash, pb_qry_factory_log);
    const auto statusResponse = status.getTxStatus(peer_ip, torii_port);
    std::cout << "Tx status=" << statusResponse << std::endl;
```

#### Transaction Batch
```c++
    IROHA_CPP::Tx tx_a(account_name, keypair);

    const auto tx_proto = tx_a.createDomain(domain_id, user_default_role)
            .createAsset(asset_name, domain_id, 0)
            .signAndAddSignature();

    GrpcResponseHandler response_handler(response_handler_log_manager);
    response_handler.handle(GrpcClient(peer_ip, torii_port, pb_qry_factory_log).sendTx(tx_proto));

    IROHA_CPP::Tx tx_b(account_name, keypair);

    const auto tx_proto = tx_b.createDomain(domain_id, user_default_role)
            .createAsset(asset_name, domain_id, 0)
            .signAndAddSignature();

    GrpcResponseHandler response_handler(response_handler_log_manager);
    response_handler.handle(GrpcClient(peer_ip, torii_port, pb_qry_factory_log).sendTx(tx_proto));

    IROHA_CPP::TxBatch tx_batch;
    tx_batch.addTransaction(tx_a);
    tx_batch.addTransaction(tx_b);

    GrpcResponseHandler response_handler(response_handler_log_manager);
    response_handler.handle(GrpcClient(peer_ip, torii_port, pb_qry_factory_log).sendTxList(tx_batch.getTxList()));
```

# C++ library for Hyperledger Iroha


This is a source repository for HL Iroha C++ library.

Currently, latest HL Iroha rc5 release (`hyperledger/iroha:latest` Docker image) is supported.


### Installation

```bash
mkdir build && cd build && cmake .. && make 
```


### Usage Example

#### Transaction

```c++
    Tx tx(account_name,
          peer_ip,
          torii_port,
          response_handler_log_manager,
          pb_qry_factory_log,
          *makeOldModel(keypair.assumeValue()),
          chr::duration_cast<chr::milliseconds>(chr::system_clock::now().time_since_epoch()).count(),
          1);

    const auto domain_id = "domainsamplev1";
    const auto user_default_role = "user";
    const auto asset_name = "assetnamesamplev1";
    std::cout << "Tx hash=" <<
                 tx.createDomain(domain_id, user_default_role)
                 .createAsset(asset_name, domain_id, 0)
                 .signAndAddSignature()
                 .send()
              << std::endl;
```

#### Transaction Batch
```c++
    auto account_name = "admin@test";
    const auto key_path = "/home/laptop/qt-workspace/iroha/example";
    const auto peer_ip = "127.0.0.1";
    int torii_port = 50051;
    const auto quorum = 1;

    const auto user_default_role = "user";

    const auto tx_a = generateTx(account_name, key_path, peer_ip, torii_port, quorum, "domainsamplev4", user_default_role, "assetnamesamplev4");
    const auto tx_b = generateTx(account_name, key_path, peer_ip, torii_port, quorum, "domainsamplev5", user_default_role, "assetnamesamplev5");

    TxBatch tx_batch(peer_ip,
                     torii_port,
                     pb_qry_factory_log,
                     response_handler_log_manager);

    tx_batch.addTransaction(tx_a.getTx());
    tx_batch.addTransaction(tx_b.getTx());
    tx_batch.send();
```

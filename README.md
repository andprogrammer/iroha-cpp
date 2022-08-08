# C++ library for Hyperledger Iroha


This is a source repository for HL Iroha C++ library.

Currently, latest HL Iroha rc5 release (`hyperledger/iroha:latest` Docker image) is supported.


### Installation

```bash
mkdir build && cd build && cmake .. && make 
```


### Usage Example

```c++
    Tx tx(account_name,
          peer_ip,
          torii_port,
          response_handler_log_manager,
          pb_qry_factory_log,
          *std::unique_ptr<iroha::keypair_t>(makeOldModel(keypair.assumeValue())),
          iroha::time::now(),
          1);

    std::string domain_id = "domainsamplev1";
    std::string user_default_role = "user";
    std::string asset_name = "assetnamesamplev1";
    tx.createDomainProto(domain_id, user_default_role)
            .createAssetProto(asset_name, domain_id, 0)
            .signAndAddSignatureProto()
            .sendProto();
```

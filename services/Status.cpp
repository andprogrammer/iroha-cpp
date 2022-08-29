#include "services/Status.hpp"

#include "GrpcClient.hpp"
#include "common/byteutils.hpp"


namespace IROHA_CPP
{

Status::Status(const std::string& tx_hash, logger::LoggerPtr pb_qry_factory_log)
    : tx_hash(tx_hash),
      pb_qry_factory_log_(std::move(pb_qry_factory_log))
{}

const Status::Response Status::getTxStatus(std::string server_ip, int server_port) const
{
    Status::Response response;
    if (iroha::hexstringToBytestring(tx_hash))
    {
        iroha::protocol::ToriiResponse answer = GrpcClient(server_ip,
                                                           server_port,
                                                           pb_qry_factory_log_)
                .getTxStatus(tx_hash)
                .answer;

        response.status_name.assign(TxStatus_Name(answer.tx_status()));
        response.status_code = answer.tx_status();
        response.error_code = answer.error_code();
    }
    return response;
}

const std::string Status::getHash() const
{
    return tx_hash;
}

}

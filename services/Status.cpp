#include "services/Status.hpp"

#include "GrpcClient.hpp"
#include "common/byteutils.hpp"


Status::Status(
        const std::string &server_ip,
        int server_port,
        std::string tx_hash,
        logger::LoggerPtr pb_qry_factory_log)
    : Request(server_ip, server_port),
      tx_hash(tx_hash),
      pb_qry_factory_log_(std::move(pb_qry_factory_log))
{}

std::string Status::getTxStatus()
{
    return getTxStatus(getServerIp(), getServerPort());
}

std::string Status::getTxStatus(std::string server_ip, int server_port)
{
    auto status = iroha::protocol::TxStatus::NOT_RECEIVED;
    iroha::protocol::ToriiResponse answer;

    if (iroha::hexstringToBytestring(tx_hash))
    {
        answer = GrpcClient(server_ip,
                            server_port,
                            pb_qry_factory_log_)
                .getTxStatus(tx_hash)
                .answer;
        status = answer.tx_status();
    }

    std::string message;
    try
    {
        message = this->userMessageMap.at(status);
        if (not answer.err_or_cmd_name().empty())
        {
            message += " error '" + answer.err_or_cmd_name() + "'";
        }
    }
    catch (const std::out_of_range& e)
    {
        message = "A problem detected while retrieving transaction status. Please "
                  "try again later.";
    }
    return message;
}

std::string Status::getHash()
{
    return tx_hash;
}

//
// Created by 杨殿擎 on 2022/10/8.
//

#ifndef DATA_INGEST_FUNCTIONS_H
#define DATA_INGEST_FUNCTIONS_H

#include <TcpReassembly.h>
#include <cstdint>
namespace tcp_reassembly {
    void msgReadyCallback(int8_t sideIndex, const pcpp::TcpStreamData& tcpData, void* userCookie);
    void connectionStartCallback(const pcpp::ConnectionData& connectionData, void* userCookie);
    void connectionEndCallback(const pcpp::ConnectionData& connectionData, pcpp::TcpReassembly::ConnectionEndReason reason, void* userCookie);
}


#endif//DATA_INGEST_FUNCTIONS_H

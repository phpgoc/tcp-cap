//
// Created by 杨殿擎 on 2022/10/8.
//

#include "functions.h"
#include "Reassembly.h"
using namespace std;
void tcp_reassembly::msgReadyCallback(int8_t sideIndex, const pcpp::TcpStreamData &tcpData, void *userCookie) {
    // extract the connection manager from the user cookie
    tcp_reassembly::Reassembly *global_cookie = (tcp_reassembly::Reassembly *) userCookie;
    //    std::cout << "tcpReassemblyMsgReadyCallback: sideIndex=" << (int)sideIndex << ", flowKey=" << tcpData.getConnectionData().flowKey << std::endl;

    // check if this flow already appears in the connection manager. If not add it
    pcpp::IPAddress ip = tcpData.getConnectionData().dstIP;

    auto id2string_iter = global_cookie->getMFlowMap().find(tcpData.getConnectionData().flowKey);
    if (id2string_iter == global_cookie->getMFlowMap().end()) {
        global_cookie->getMFlowMap().insert(std::make_pair(tcpData.getConnectionData().flowKey, AssemblyData()));
        id2string_iter = global_cookie->getMFlowMap().find(tcpData.getConnectionData().flowKey);
    }
    if(sideIndex == 1)
        return;

    id2string_iter->second.getMData().append((char *) tcpData.getData(), tcpData.getDataLength());
}

void tcp_reassembly::connectionStartCallback(const pcpp::ConnectionData &connectionData, void *userCookie) {
    // get a pointer to the connection manager
    tcp_reassembly::Reassembly *global_cookie = (tcp_reassembly::Reassembly *) userCookie;
    std::cout << "New connection: " << connectionData.flowKey << std::endl;

    auto id2string_iter = global_cookie->getMFlowMap().find(connectionData.flowKey);
    if (id2string_iter == global_cookie->getMFlowMap().end()) {
        global_cookie->getMFlowMap().insert(std::make_pair(connectionData.flowKey, AssemblyData()));
    }
    global_cookie->incr_start_count();
}


/**
 * The callback being called by the TCP reassembly module whenever a connection is ending. This method removes the connection from the connection manager and writes the metadata file if requested
 * by the user
 */
void tcp_reassembly::connectionEndCallback(const pcpp::ConnectionData &connectionData, pcpp::TcpReassembly::ConnectionEndReason reason, void *userCookie) {

    tcp_reassembly::Reassembly *global_cookie = (tcp_reassembly::Reassembly *) userCookie;
    pcpp::IPAddress ip = connectionData.dstIP;
    std::cout << "Connection " << connectionData.flowKey << " ended. Reason: " << reason << std::endl;

    auto id2string_iter = global_cookie->getMFlowMap().find(connectionData.flowKey);
    if (id2string_iter == global_cookie->getMFlowMap().end()) {
        cerr << "程序错误" << endl;
        exit(1);
    }
    global_cookie->incr_end_count();
    cout << global_cookie->getMFlowMap().find(connectionData.flowKey)->second.getMData() << std::endl;
    global_cookie->getMFlowMap().erase(id2string_iter);
}
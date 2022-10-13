//
// Created by 杨殿擎 on 2022/10/8.
//

#include "functions.h"
#include "../protocol/Entrance.h"
#include "DeleteQueue.h"
#include "Reassembly.h"
using namespace std;
void tcp_reassembly::msgReadyCallback(int8_t sideIndex, const pcpp::TcpStreamData &tcpData, void *userCookie) {
    tcp_reassembly::ReassemblyDataInDevice *cookie = (tcp_reassembly::ReassemblyDataInDevice *) userCookie;
    if (sideIndex == 1)
        return;
    auto assembly_data_iter = Reassembly::getInstance()->getMFlowMap().find(cookie->getMDeviceIp());
    if (assembly_data_iter == Reassembly::getInstance()->getMFlowMap().end()) {
        cerr << "程序运行错误" << endl;
        exit(1);
    }

    AssemblyData *data = Reassembly::getInstance()->get_data_pointer_from_flow_map(
            cookie->getMDeviceIp(), tcpData.getConnectionData().flowKey);

    if (data == nullptr) {
        assembly_data_iter->second.insert(std::make_pair(tcpData.getConnectionData().flowKey, AssemblyData(tcpData.getConnectionData().endTime)));
        cerr << "compatibility_tcp_max_time设置低了，tcp connention 心跳，消息队列阻塞长度，等因数共同决定。不影响正常逻辑" << endl;
        cerr << "flow key:" << tcpData.getConnectionData().flowKey << endl;
        data = Reassembly::getInstance()->get_data_pointer_from_flow_map(cookie->getMDeviceIp(),
                                                                         tcpData.getConnectionData().flowKey);
    }
    if (data->getMEndTime().tv_sec < tcpData.getConnectionData().endTime.tv_sec) {
        if (0 == data->getMEndTime().tv_sec) {
            add_2_delete_queue(tcpData.getConnectionData().flowKey, tcpData.getConnectionData().endTime, cookie->getMDeviceIp());
        }
        data->setMEndTime(tcpData.getConnectionData().endTime);
    }
    data->getMData().append((char *) tcpData.getData(), tcpData.getDataLength());
}

void tcp_reassembly::connectionStartCallback(const pcpp::ConnectionData &connectionData, void *userCookie) {
    // get a pointer to the connection manager
    std::cout << "New connection: " << connectionData.flowKey << std::endl;

    tcp_reassembly::ReassemblyDataInDevice *cookie = (tcp_reassembly::ReassemblyDataInDevice *) userCookie;
    auto assembly_data_iter = Reassembly::getInstance()->getMFlowMap().find(cookie->getMDeviceIp());
    if (assembly_data_iter == Reassembly::getInstance()->getMFlowMap().end()) {
        cerr << "程序运行错误" << endl;
        exit(1);
    }
    auto id2string_iter = assembly_data_iter->second.find(connectionData.flowKey);
    if (id2string_iter == assembly_data_iter->second.end()) {
        assembly_data_iter->second.insert(std::make_pair(connectionData.flowKey, AssemblyData({})));
    }
    Reassembly::getInstance()->incr_start_count();
}


/**
 * The callback being called by the TCP reassembly module whenever a connection is ending. This method removes the connection from the connection manager and writes the metadata file if requested
 * by the user
 */
void tcp_reassembly::connectionEndCallback(const pcpp::ConnectionData &connectionData, pcpp::TcpReassembly::ConnectionEndReason reason, void *userCookie) {

    tcp_reassembly::ReassemblyDataInDevice *cookie = (tcp_reassembly::ReassemblyDataInDevice *) userCookie;

    std::cout << "Connection " << connectionData.flowKey << " ended. Reason: " << reason << std::endl;
    auto assembly_data_iter = Reassembly::getInstance()->getMFlowMap().find(cookie->getMDeviceIp());
    if (assembly_data_iter == Reassembly::getInstance()->getMFlowMap().end()) {
        cerr << "程序运行错误" << endl;
        exit(1);
    }

    cout << "start count: " << Reassembly::getInstance()->getMStartCount() << endl;
    cout << "end count: " << Reassembly::getInstance()->getMEndCount() << endl;
    cout << "error count: " << Reassembly::getInstance()->getMErrorCount() << endl;
    auto map = Reassembly::getInstance()->getAssemblyDataByIP(cookie->getMDeviceIp());
    if (Reassembly::find_in_flow_map_handle_earse_with_lock(map, connectionData.flowKey)) {
        Reassembly::getInstance()->incr_end_count();
    }
}
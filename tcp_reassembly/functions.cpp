//
// Created by 杨殿擎 on 2022/10/8.
//

#include "functions.h"
#include "Reassembly.h"
using namespace std;
void tcp_reassembly::msgReadyCallback(int8_t sideIndex, const pcpp::TcpStreamData &tcpData, void *userCookie) {
    tcp_reassembly::ReassemblyDataInDevice *global_cookie = (tcp_reassembly::ReassemblyDataInDevice *) userCookie;
    if(sideIndex == 1)
        return;
    auto assembly_data_iter = global_cookie->getMpGlobalReassembly()->getMMutiFlowMap().find(global_cookie->getMDeviceIp());
    if (assembly_data_iter == global_cookie->getMpGlobalReassembly()->getMMutiFlowMap().end()) {
        cerr << "程序运行错误" << endl;
        exit(1);
    }

    string * data_string = global_cookie->getMpGlobalReassembly()->get_data_string_pointer_from_muti_flow_map(global_cookie->getMDeviceIp(),tcpData.getConnectionData().flowKey);

    if(data_string == nullptr){
        assembly_data_iter->second.insert(std::make_pair(tcpData.getConnectionData().flowKey, AssemblyData()));
        cerr << "不该到这里" << endl;
    }
    data_string-> append((char *) tcpData.getData(), tcpData.getDataLength());
}

void tcp_reassembly::connectionStartCallback(const pcpp::ConnectionData &connectionData, void *userCookie) {
    // get a pointer to the connection manager
    std::cout << "New connection: " << connectionData.flowKey << std::endl;

    tcp_reassembly::ReassemblyDataInDevice *global_cookie = (tcp_reassembly::ReassemblyDataInDevice *) userCookie;
    auto assembly_data_iter = global_cookie->getMpGlobalReassembly()->getMMutiFlowMap().find(global_cookie->getMDeviceIp());
    if (assembly_data_iter == global_cookie->getMpGlobalReassembly()->getMMutiFlowMap().end()) {
        cerr << "程序运行错误" << endl;
        exit(1);
    }
    auto id2string_iter = assembly_data_iter->second.find(connectionData.flowKey);
    if (id2string_iter == assembly_data_iter->second.end()) {
        assembly_data_iter->second.insert(std::make_pair(connectionData.flowKey, AssemblyData()));
    }
    global_cookie->getMpGlobalReassembly()->incr_start_count();
}


/**
 * The callback being called by the TCP reassembly module whenever a connection is ending. This method removes the connection from the connection manager and writes the metadata file if requested
 * by the user
 */
void tcp_reassembly::connectionEndCallback(const pcpp::ConnectionData &connectionData, pcpp::TcpReassembly::ConnectionEndReason reason, void *userCookie) {

    tcp_reassembly::ReassemblyDataInDevice *global_cookie = (tcp_reassembly::ReassemblyDataInDevice *) userCookie;

    std::cout << "Connection " << connectionData.flowKey << " ended. Reason: " << reason << std::endl;
    auto assembly_data_iter = global_cookie->getMpGlobalReassembly()->getMMutiFlowMap().find(global_cookie->getMDeviceIp());
    if (assembly_data_iter == global_cookie->getMpGlobalReassembly()->getMMutiFlowMap().end()) {
        cerr << "程序运行错误" << endl;
        exit(1);
    }

    global_cookie->getMpGlobalReassembly()->incr_end_count();
    cout << *(global_cookie->getMpGlobalReassembly()->get_data_string_pointer_from_muti_flow_map(global_cookie->getMDeviceIp(),connectionData.flowKey)) << std::endl;
    global_cookie->getMpGlobalReassembly()->earse_from_muti_flow_map(global_cookie->getMDeviceIp(), connectionData.flowKey);
}
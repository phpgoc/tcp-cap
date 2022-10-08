//
// Created by 杨殿擎 on 2022/10/8.
//

#include "Reassembly.h"
#include "functions.h"
using namespace std;
namespace tcp_reassembly {
    Reassembly *Reassembly::getInstance() {
        static Reassembly instance;
        return &instance;
    }

    Reassembly::Reassembly() {
        m_start_count = 0;
        m_end_count = 0;
        m_flow_map.clear();
        m_tcp_reassembly_map.clear();
    }

    Reassembly::~Reassembly() {

    }

    void Reassembly::add_2_tcp_reassembly_map(const pcpp::IPAddress& ip) {

        m_flow_map.insert(pair(ip,  std::map<uint16_t, AssemblyData>{}));
        m_tcp_reassembly_map.insert(std::pair<pcpp::IPAddress, pcpp::TcpReassembly>(ip, pcpp::TcpReassembly(
                                                                                                msgReadyCallback, this,connectionStartCallback, connectionEndCallback )));
    }
     std::map<pcpp::IPAddress, pcpp::TcpReassembly> &Reassembly::getMTcpReassemblyMap()  {
        return m_tcp_reassembly_map;
    }
     std::map<pcpp::IPAddress, std::map<uint16_t, AssemblyData>> &Reassembly::getMFlowMap()  {
        return m_flow_map;
    }
     int Reassembly::getMStartCount() const {
         return m_start_count;
     }
     int Reassembly::getMEndCount() const {
         return m_end_count;
     }
}
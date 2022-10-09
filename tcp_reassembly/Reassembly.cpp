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

    pcpp::TcpReassembly *Reassembly::getGlobalTcpReassembly() {
        static pcpp::TcpReassembly globalTcpReassembly(msgReadyCallback, Reassembly::getInstance(), connectionStartCallback, connectionEndCallback);
        return &globalTcpReassembly;
    }
    Reassembly::Reassembly() {
        m_start_count = 0;
        m_end_count = 0;
    }

    Reassembly::~Reassembly() {
    }

    void Reassembly::add_2_tcp_reassembly_map(const pcpp::IPAddress &ip) {
        m_muti_flow_map.insert(pair(ip, std::map<uint16_t, AssemblyData>{}));
        ReassemblyDataInDevice* cookie = new ReassemblyDataInDevice{this,ip};
        m_tcp_reassembly_map.insert(std::pair<pcpp::IPAddress, pcpp::TcpReassembly>(ip, pcpp::TcpReassembly(
                                                                                                msgReadyCallback, cookie, connectionStartCallback, connectionEndCallback)));
    }
    std::map<pcpp::IPAddress, pcpp::TcpReassembly> &Reassembly::getMTcpReassemblyMap() {
        return m_tcp_reassembly_map;
    }
    std::map<uint16_t, AssemblyData> &Reassembly::getMFlowMap() {
        return m_flow_map;
    }
    std::map<pcpp::IPAddress, std::map<uint16_t, AssemblyData>> &Reassembly::getMMutiFlowMap() {
        return m_muti_flow_map;
    }
    int Reassembly::getMStartCount() const {
        return m_start_count;
    }
    int Reassembly::getMEndCount() const {
        return m_end_count;
    }

    bool Reassembly::earse_from_muti_flow_map(const pcpp::IPAddress &ip, uint16_t flowKey) {
        auto it = m_muti_flow_map.find(ip);
        if (it != m_muti_flow_map.end()) {
            auto it2 = it->second.find(flowKey);
            if (it2 != it->second.end()) {
                it->second.erase(it2);
                return true;
            }
        }
        return false;
    }
    std::string* Reassembly::get_data_string_pointer_from_muti_flow_map(const pcpp::IPAddress &ip, uint16_t flowKey) {

        auto it = m_muti_flow_map.find(ip);
        if (it != m_muti_flow_map.end()) {
            auto it2 = it->second.find(flowKey);
            if (it2 != it->second.end()) {
                return &it2->second.getMData();
            }
        }
        return nullptr;
    }

    ReassemblyDataInDevice::ReassemblyDataInDevice( Reassembly* global_reassembly,  pcpp::IPAddress ip)
    {
        mp_global_reassembly = global_reassembly;
        m_device_ip = ip;
    }
    Reassembly *ReassemblyDataInDevice::getMpGlobalReassembly() const {
        return mp_global_reassembly;
    }

    const pcpp::IPAddress &ReassemblyDataInDevice::getMDeviceIp() const {
        return m_device_ip;
    }

}// namespace tcp_reassembly
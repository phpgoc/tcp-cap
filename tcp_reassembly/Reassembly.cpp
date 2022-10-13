//
// Created by 杨殿擎 on 2022/10/8.
//

#include "Reassembly.h"
#include "../protocol/Entrance.h"
#include "functions.h"

using namespace std;

namespace tcp_reassembly {
    mutex Reassembly::s_mutex_for_erase_flow_from_map;

    Reassembly *Reassembly::getInstance() {
        static Reassembly instance;
        return &instance;
    }

    bool Reassembly::find_in_flow_map_handle_earse_with_lock(std::map<uint16_t, AssemblyData> *map, uint16_t flowKey) {
        std::lock_guard<std::mutex> lock(s_mutex_for_erase_flow_from_map);
        auto it = map->find(flowKey);
        if (it != map->end()) {
            protocol::Entrance::handle(map->at(flowKey).getMData());
            map->erase(it);
            cout << "error numuber: " << Reassembly::getInstance()->getMErrorCount() << endl;
            return true;
        }
        return false;
    }


    Reassembly::Reassembly() {
        m_start_count = 0;
        m_end_count = 0;
    }

    Reassembly::~Reassembly() {
    }

    void Reassembly::add_2_tcp_reassembly_map(const pcpp::IPAddress &ip) {
        m_flow_map.insert(pair(ip, std::map<uint16_t, AssemblyData>{}));
        ReassemblyDataInDevice *cookie = new ReassemblyDataInDevice{ip}; //泄漏没多少，全局的。有多少个
        m_tcp_reassembly_map.insert(std::pair<pcpp::IPAddress, pcpp::TcpReassembly>(ip, pcpp::TcpReassembly(
                                                                                                msgReadyCallback, cookie, connectionStartCallback, connectionEndCallback)));
    }

    std::map<pcpp::IPAddress, pcpp::TcpReassembly> &Reassembly::getMTcpReassemblyMap() {
        return m_tcp_reassembly_map;
    }

    std::map<pcpp::IPAddress, std::map<uint16_t, AssemblyData>> &Reassembly::getMFlowMap() {
        return m_flow_map;
    }

    int Reassembly::getMStartCount() const {
        return m_start_count;
    }

    int Reassembly::getMEndCount() const {
        return m_end_count;
    }

    AssemblyData *Reassembly::get_data_pointer_from_flow_map(const pcpp::IPAddress &ip, uint16_t flowKey) {

        auto it = m_flow_map.find(ip);
        if (it != m_flow_map.end()) {
            auto it2 = it->second.find(flowKey);
            if (it2 != it->second.end()) {
                return &it2->second;
            }
        }
        return nullptr;
    }

    std::map<uint16_t, AssemblyData> *Reassembly::getAssemblyDataByIP(pcpp::IPAddress ip) {
        return &this->getMFlowMap().find(ip)->second;
    }


    ReassemblyDataInDevice::ReassemblyDataInDevice(pcpp::IPAddress ip) {
        m_device_ip = ip;
    }


    const pcpp::IPAddress &ReassemblyDataInDevice::getMDeviceIp() const {
        return m_device_ip;
    }

    int Reassembly::getMErrorCount() const {
        return m_error_count;
    }
}// namespace tcp_reassembly
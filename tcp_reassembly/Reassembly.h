//
// Created by 杨殿擎 on 2022/10/8.
//

#ifndef DATA_INGEST_REASSEMBLY_H
#define DATA_INGEST_REASSEMBLY_H
#include "AssemblyData.h"
#include "TcpReassembly.h"
#include <iostream>
#include <mutex>
namespace tcp_reassembly {


    class Reassembly {
    public:
        static std::mutex s_mutex_for_erase_flow_from_map;
        static bool find_in_flow_map_handle_earse_with_lock(std::map<uint16_t, AssemblyData> *map, uint16_t flowKey);
        static Reassembly *getInstance();
        ~Reassembly();

        void add_2_tcp_reassembly_map(const pcpp::IPAddress &ip);

        std::map<pcpp::IPAddress, pcpp::TcpReassembly> &getMTcpReassemblyMap();

        std::map<pcpp::IPAddress, std::map<uint16_t, AssemblyData>> &getMFlowMap();
        AssemblyData *get_data_pointer_from_flow_map(const pcpp::IPAddress &ip, uint16_t flowKey);
        std::map<uint16_t, AssemblyData> *getAssemblyDataByIP(pcpp::IPAddress);
        inline void incr_start_count() {
            m_start_count++;
        }
        inline void incr_end_count() {
            m_end_count++;
        }
        inline void incr_error_count() {
            m_error_count++;
        }
        int getMStartCount() const;
        int getMEndCount() const;
        int getMErrorCount() const;

    private:
        Reassembly();
        int m_start_count;
        int m_end_count;
        int m_error_count;
        std::map<pcpp::IPAddress, pcpp::TcpReassembly> m_tcp_reassembly_map;
        std::map<pcpp::IPAddress, std::map<uint16_t, AssemblyData>> m_flow_map;
    };

    class ReassemblyDataInDevice {
    public:
        ReassemblyDataInDevice( pcpp::IPAddress ip);
        const pcpp::IPAddress &getMDeviceIp() const;
    private:
        pcpp::IPAddress m_device_ip;
    };
}// namespace tcp_reassembly


#endif//DATA_INGEST_REASSEMBLY_H

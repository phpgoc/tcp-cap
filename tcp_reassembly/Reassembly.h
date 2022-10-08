//
// Created by 杨殿擎 on 2022/10/8.
//

#ifndef DATA_INGEST_REASSEMBLY_H
#define DATA_INGEST_REASSEMBLY_H
#include "TcpReassembly.h"
#include "AssemblyData.h"
#include <iostream>

namespace tcp_reassembly {
    class Reassembly {
    public:
        static Reassembly *getInstance();
        ~Reassembly();
        void add_2_tcp_reassembly_map(const pcpp::IPAddress &ip);
        std::map<pcpp::IPAddress, pcpp::TcpReassembly> &getMTcpReassemblyMap();
        std::map<pcpp::IPAddress, std::map<uint16_t, AssemblyData>> &getMFlowMap();
        inline void incr_start_count() {
            m_start_count++;
        }
        inline void incr_end_count() {
            m_end_count++;
        }
        int getMStartCount() const;
        int getMEndCount() const;

    private:
        Reassembly();
        int m_start_count;
        int m_end_count;
        std::map<pcpp::IPAddress, pcpp::TcpReassembly> m_tcp_reassembly_map;
        std::map<pcpp::IPAddress, std::map<uint16_t, AssemblyData>> m_flow_map;
    };
}// namespace tcp_reassembly


#endif//DATA_INGEST_REASSEMBLY_H

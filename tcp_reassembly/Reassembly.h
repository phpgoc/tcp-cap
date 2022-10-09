//
// Created by 杨殿擎 on 2022/10/8.
//

#ifndef DATA_INGEST_REASSEMBLY_H
#define DATA_INGEST_REASSEMBLY_H
#include "AssemblyData.h"
#include "TcpReassembly.h"
#include <iostream>

namespace tcp_reassembly {


    class Reassembly {
    public:
        static Reassembly *getInstance();
        static pcpp::TcpReassembly *getGlobalTcpReassembly();
        ~Reassembly();
        /**
         * 由于多个TcpAssembly对象，组包失败，这个方法暂时不用
         * @return
         */
        void add_2_tcp_reassembly_map(const pcpp::IPAddress &ip);
        /**
         * 由于多个TcpAssembly对象，组包失败，这个方法暂时不用，使用getGlobalTcpReassembly
         * @return
         */
        std::map<pcpp::IPAddress, pcpp::TcpReassembly> &getMTcpReassemblyMap();
        /**
         * 由于多个TcpAssembly对象，组包失败，这个方法暂时不用，使用getGlobalTcpReassembly
         * @return
         */
        std::map<uint16_t, AssemblyData> &getMFlowMap();
        /**
         * 由于多个TcpAssembly对象，组包失败，这个方法暂时不用，使用getGlobalTcpReassembly
         * @return
         */
        std::map<pcpp::IPAddress, std::map<uint16_t, AssemblyData>> &getMMutiFlowMap();
        std::string* get_data_string_pointer_from_muti_flow_map(const pcpp::IPAddress &ip, uint16_t flowKey);
        bool earse_from_muti_flow_map(const pcpp::IPAddress &ip, uint16_t flowKey);
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
        std::map<uint16_t, AssemblyData> m_flow_map;
        std::map<pcpp::IPAddress, std::map<uint16_t, AssemblyData>> m_muti_flow_map;
    };

    class ReassemblyDataInDevice {
    public:
        ReassemblyDataInDevice( Reassembly* global_reassembly,  pcpp::IPAddress ip);

        Reassembly *getMpGlobalReassembly() const;
        void setMpGlobalReassembly(Reassembly *mpGlobalReassembly);
        const pcpp::IPAddress &getMDeviceIp() const;
        void setMDeviceIp(const pcpp::IPAddress &mDeviceIp);

    private:
        Reassembly* mp_global_reassembly;
        pcpp::IPAddress m_device_ip;
    };
}// namespace tcp_reassembly


#endif//DATA_INGEST_REASSEMBLY_H

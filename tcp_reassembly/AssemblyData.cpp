//
// Created by 杨殿擎 on 2022/10/8.
//

#include "AssemblyData.h"
tcp_reassembly::AssemblyData::AssemblyData(timeval endTime) : m_end_time(endTime), m_data("") {}


std::string &tcp_reassembly::AssemblyData::getMData() {
    return m_data;
}
void tcp_reassembly::AssemblyData::setMData(const std::string &mData) {
    m_data = mData;
}
const timeval &tcp_reassembly::AssemblyData::getMEndTime() const {
    return m_end_time;
}
void tcp_reassembly::AssemblyData::setMEndTime(const timeval &endTime) {
    m_end_time = endTime;
}

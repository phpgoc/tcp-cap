//
// Created by 杨殿擎 on 2022/10/8.
//

#include "AssemblyData.h"
tcp_reassembly::AssemblyData::AssemblyData() {
    m_data = "";
    side = 0;
}

 std::string &tcp_reassembly::AssemblyData::getMData()  {
    return m_data;
}
void tcp_reassembly::AssemblyData::setMData(const std::string &mData) {
    m_data = mData;
}
int tcp_reassembly::AssemblyData::getSide() const {
    return side;
}
void tcp_reassembly::AssemblyData::setSide(int side) {
    AssemblyData::side = side;
}

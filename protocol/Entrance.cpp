//
// Created by 杨殿擎 on 2022/10/10.
//

#include "Entrance.h"
#include "../tcp_reassembly/Reassembly.h"
namespace protocol {
    void Entrance::handle(const std::string &b) {
                std::cout << "Entrance::handle: " << b << std::endl;
    }
    Entrance::Entrance() {}
    Entrance::~Entrance() {}
}// namespace protocol
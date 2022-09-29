//
// Created by 杨殿擎 on 2022/9/29.
//

#ifndef VCPKG_1_FACTORY_H
#define VCPKG_1_FACTORY_H
#include "Base.h"
#include <string>
namespace server_drvier {
    server_drvier::Base *get_server_instance(const std::string &type, const std::string &ip, int port);
}// namespace server_drvier


#endif//VCPKG_1_FACTORY_H

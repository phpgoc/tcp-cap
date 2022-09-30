//
// Created by 杨殿擎 on 2022/9/29.
//

#ifndef VCPKG_1_TOOLS_H
#define VCPKG_1_TOOLS_H
#include <iostream>
static inline void check_server_type_valid(const std::string &config_file, const std::string &server_type) {
    if (server_type.empty()) {
        std::cerr << "Parsing failed:" << config_file << ":server.type is invalid"
                  << "\n";
        exit(1);
    }
}

static inline void check_server_ip_valid(const std::string &config_file, const std::string *server_type) {

    if (server_type->empty()) {
        std::cerr << "Parsing failed:" << config_file << ":server.ip is invalid"
                  << "\n";
        exit(1);
    }
}

static inline void check_server_port_valid(const std::string &config_file, int server_port) {
    if (0 == server_port) {
        std::cerr << "Parsing failed: " << config_file << ":server.port is invalid"
                  << "\n";
        exit(1);
    }
}
#endif//VCPKG_1_TOOLS_H

//
// Created by 杨殿擎 on 2022/9/29.
//

#ifndef VCPKG_1_BASE_H
#define VCPKG_1_BASE_H
#include <string>
namespace server_drvier {

    class Base {
    public:
        virtual void push(const std::string &b) = 0;
        virtual void pull_loop(void (*handle)(const std::string &)) = 0;
        virtual ~Base() {}
    };
}// namespace server_drvier

#endif//VCPKG_1_BASE_H

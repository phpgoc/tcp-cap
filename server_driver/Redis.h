//
// Created by 杨殿擎 on 2022/9/29.
//

#ifndef VCPKG_1_REDIS_H
#define VCPKG_1_REDIS_H
#include "Base.h"
#include <sw/redis++/redis++.h>
namespace server_drvier {


    class Redis : public Base {
    public:
        Redis(const std::string &ip, int port, std::string message_queue);
        ~Redis();
        void push(const std::string &b) override;
        void pull_loop(void (*handle)(const std::string &), bool *stop) override;

    private:
        sw::redis::Redis *mp_client = nullptr;
    };

}// namespace server_drvier

#endif//VCPKG_1_REDIS_H

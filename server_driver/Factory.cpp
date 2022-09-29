//
// Created by 杨殿擎 on 2022/9/29.
//

#include "Factory.h"
#include "Nats.h"
#include "Redis.h"
server_drvier::Base *server_drvier::get_server_instance(const std::string &type,
                                                        const std::string &ip, int port) {
    if (type == "redis") {
        return new Redis(ip, port);
    } else if (type == "nats") {
        return new Nats(ip, port);
    }
    return new Redis(ip, port);// test
}
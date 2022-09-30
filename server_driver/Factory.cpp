//
// Created by 杨殿擎 on 2022/9/29.
//

#include "Factory.h"
#include "Nats.h"
#include "Redis.h"
#include <iostream>
using namespace std;
server_drvier::Base *server_drvier::get_server_instance(const std::string &type,
                                                        const std::string &ip, int port, const std::string &message_queue) {
    if (type == "redis") {
        return new Redis(ip, port, message_queue);
    } else if (type == "nats") {
        return new Nats(ip, port, message_queue);
    }
    cerr << "not support type: " << type << endl;
    cerr << "support type: redis, nats" << endl;
    exit(1);
}
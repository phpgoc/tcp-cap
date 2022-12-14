//
// Created by 杨殿擎 on 2022/9/29.
//

#include "Redis.h"
#include <iostream>
using namespace std;
server_driver::Redis::Redis(const string &ip, int port, std::string message_queue) : Base(message_queue) {
    string url = "tcp://" + string(ip) + ":" + to_string(port);
    sw::redis::ConnectionOptions connection_options(url);
    connection_options.keep_alive = true;
    connection_options.connect_timeout = std::chrono::milliseconds(100);
    sw::redis::ConnectionPoolOptions pool_options;

    pool_options.wait_timeout = std::chrono::milliseconds(1);
    mp_client = new sw::redis::Redis(connection_options, pool_options);
}
server_driver::Redis::~Redis() {
    delete mp_client;
}
void server_driver::Redis::push(const string &b) {
    mp_client->lpush(this->getMessageQueue(), b);
}
void server_driver::Redis::pull_loop(void (*handle)(const string &), bool *stop) {
    while (!(*stop)) {
        auto result = mp_client->brpop(this->getMessageQueue(), 10);
        if (result) {
            handle(result.value().second);
        }
    }
}

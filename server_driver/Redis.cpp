//
// Created by 杨殿擎 on 2022/9/29.
//

#include "Redis.h"
#include <string>
using namespace std;
server_drvier::Redis::Redis(const string &ip, int port) {
    string url = "tcp://" + string(ip) + ":" + to_string(port);
    sw::redis::ConnectionOptions connection_options(url);
    connection_options.keep_alive = true;
    connection_options.connect_timeout = std::chrono::milliseconds(100);
    sw::redis::ConnectionPoolOptions pool_options;
    
    pool_options.wait_timeout = std::chrono::milliseconds(1);
    m_client = new sw::redis::Redis(connection_options, pool_options);
}
server_drvier::Redis::~Redis() {
    delete m_client;
}
void server_drvier::Redis::push(const string &b) {
    m_client->lpush("message", b);
}

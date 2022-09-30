//
// Created by 杨殿擎 on 2022/9/30.
//
#include "PcapLiveDeviceList.h"
#include "ServerConfig.h"
#include "SystemUtils.h"
#include "server_driver/Factory.h"
#include <iostream>
using namespace std;

void handle(const string &b) {
    //    cout << "handle: " << b << endl;
    char *p = const_cast<char *>(b.data());
    int *dataLen = (int *) p;
    p += sizeof(int);
    timespec *time = (timespec *) p;
    p += sizeof(timespec);
    const uint8_t *data = (const uint8_t *) p;
    pcpp::RawPacket packet{data, *dataLen, *time, false};
    pcpp::Packet parsedPacket(&packet);
    cout << parsedPacket.toString() << endl;
}

int main(int argc, char *argv[]) {
    ServerConfig c;
    c.debug();
    server_drvier::Base *server = server_drvier::get_server_instance(c.getMServerType(), c.getMServerIp(), c.getMServerPort());
    auto safe_quit_process = [](int) {
        printf("safe quit");
    };
    signal(SIGTERM, safe_quit_process);
    signal(SIGINT, safe_quit_process);
    server->pull_loop(handle);
    return 0;
}
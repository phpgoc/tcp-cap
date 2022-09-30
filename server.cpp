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
    char *ptr = const_cast<char *>(b.data());
    int *dataLen = (int *) ptr;
    ptr += sizeof(int);
    timespec *time = (timespec *) ptr;
    ptr += sizeof(timespec);
    const uint8_t *data = (const uint8_t *) ptr;
    pcpp::RawPacket packet{data, *dataLen, *time, false};
    pcpp::Packet parsedPacket(&packet);
    cout << parsedPacket.toString() << endl;
}

int main(int argc, char *argv[]) {
    ServerConfig config;
    if (argc > 1) {
        if (string(argv[1]) == "-h") {
            printf("help");
            return 0;
        }
        config.set_config(argv[1]);
    } else {
        config.set_config(nullptr);
    }
    config.debug();
    server_drvier::Base *server = server_drvier::get_server_instance(config.getMServerType(), config.getMServerIp(), config.getMServerPort(), config.getMMessageQueue());
    auto safe_quit_process = [](int) {
        printf("safe quit");
    };
    signal(SIGTERM, safe_quit_process);
    signal(SIGINT, safe_quit_process);
    server->pull_loop(handle);
    return 0;
}
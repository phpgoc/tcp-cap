//
// Created by 杨殿擎 on 2022/9/30.
//
#include "Packet.h"
#include "ServerConfig.h"
#include "server_driver/Factory.h"
#include "tcp_reassembly/DeleteQueue.h"
#include "tcp_reassembly/Reassembly.h"
#include <iostream>
using namespace std;

void handle(const string &b) {
    //    cout << "handle: " << b << endl;
    char *ptr = const_cast<char *>(b.data());
    int *dataLen = (int *) ptr;
    ptr += sizeof(int);
    timespec *time = (timespec *) ptr;
    ptr += sizeof(timespec);
    pcpp::IPAddress *devIP = (pcpp::IPAddress *) ptr;
    ptr += sizeof(pcpp::IPAddress);
    const uint8_t *data = (const uint8_t *) ptr;
    pcpp::RawPacket packet{data, *dataLen, *time, false};
    auto reassembly = tcp_reassembly::Reassembly::getInstance()->getMTcpReassemblyMap().find(*devIP);
    if (reassembly == tcp_reassembly::Reassembly::getInstance()->getMTcpReassemblyMap().end()) {
        tcp_reassembly::Reassembly::getInstance()->add_2_tcp_reassembly_map(*devIP);
        reassembly = tcp_reassembly::Reassembly::getInstance()->getMTcpReassemblyMap().find(*devIP);
    }
    reassembly->second.reassemblePacket(&packet);
    //    pcpp::Packet parsedPacket(&packet);
    //    cout << parsedPacket.toString() << endl;
}
bool shouldStop = false;
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
        shouldStop = true;
    };
    signal(SIGTERM, safe_quit_process);
    signal(SIGINT, safe_quit_process);
    //todo 可以设定一定延迟再启动 delete_queue ,防止消息队列里堆积的消息满足delete_queue删除条件，直接被删除
    tcp_reassembly::delete_queue_thread(&shouldStop);
    server->pull_loop(handle, &shouldStop);

    printf("safe quit");
    return 0;
}
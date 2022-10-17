//
// Created by 杨殿擎 on 2022/9/30.
//
#include "Packet.h"
#include "SSLLayer.h"
#include "SSHLayer.h"
#include "ServerConfig.h"
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
    pcpp::Packet parsedPacket(&packet);

    //过滤ssl包
    pcpp::SSLLayer *ssl_layer = parsedPacket.getLayerOfType<pcpp::SSLLayer>();
    if(ssl_layer != nullptr){
        return;
    }
    pcpp::SSHLayer *ssh_layer = parsedPacket.getLayerOfType<pcpp::SSHLayer>();
//        cout << parsedPacket.toString() << endl;

    auto reassembly = tcp_reassembly::Reassembly::getInstance()->getMTcpReassemblyMap().find(*devIP);
    if (reassembly == tcp_reassembly::Reassembly::getInstance()->getMTcpReassemblyMap().end()) {
        tcp_reassembly::Reassembly::getInstance()->add_2_tcp_reassembly_map(*devIP);
        reassembly = tcp_reassembly::Reassembly::getInstance()->getMTcpReassemblyMap().find(*devIP);
    }
    reassembly->second.reassemblePacket(&packet);
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
    config.info();
    server_driver::Base *server = get_message_server_instance(config);
    auto safe_quit_process = [](int) {
        shouldStop = true;
    };
    signal(SIGTERM, safe_quit_process);
    signal(SIGINT, safe_quit_process);

    tcp_reassembly::delete_queue_thread(&shouldStop);
    server->pull_loop(handle, &shouldStop);

    printf("safe quit");
    return 0;
}
#include "Config.h"
#include "server_driver/Factory.h"
#include <iostream>
#include <unistd.h>
using namespace std;
#include "PcapLiveDeviceList.h"
#include "SystemUtils.h"

static bool shouldStop = false;

static void on_packet_arrives(pcpp::RawPacket *packet, pcpp::PcapLiveDevice *dev, void *cookie) {
    // extract the stats object form the cookie
    server_drvier::Base *server = (server_drvier::Base *) cookie;
    // parsed the raw packet
    pcpp::Packet parsedPacket(packet);
    cout << parsedPacket.toString() << endl;
    char *base_ptr = (char *) malloc(sizeof(int) + sizeof(timespec) + packet->getRawDataLen());
    char *ptr = base_ptr;
    int *pInt = (int *) ptr;
    *pInt = packet->getRawDataLen();
    ptr += sizeof(int);
    timespec *time = (timespec *) ptr;
    *time = packet->getPacketTimeStamp();
    ptr += sizeof(timespec);
    memcpy(ptr, packet->getRawData(), packet->getRawDataLen());
    server->push(string(base_ptr, sizeof(int) + sizeof(timespec) + packet->getRawDataLen()));
    free(base_ptr);
}
int main(int argc, char *argv[]) {
    Config config;
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
    pcpp::OrFilter or_filter;
    vector<void *> filterPointersForDeletion;
    for (auto i : config.getMDbs()) {
        pcpp::PortFilter *portFilter = new pcpp::PortFilter(i.first, pcpp::SRC_OR_DST);// 测试环境为了多一点数据

        //        pcpp::PortFilter *portFilter = new pcpp::PortFilter(i.first, pcpp::DST);//也许生产只需要dst,因为只有dst才能保证数据包是从外部到内部的
        filterPointersForDeletion.push_back(portFilter);
        or_filter.addFilter(portFilter);
    }
    pcpp::AndFilter and_filter;
    and_filter.addFilter(&or_filter);
    pcpp::ProtoFilter protocol_filter(pcpp::TCP);
    and_filter.addFilter(&protocol_filter);

    config.getMDevice()->setFilter(and_filter);
    config.getMDevice()->startCapture(on_packet_arrives, server);

    auto safe_quit_process = [](int) {
        shouldStop = true;
    };
    signal(SIGTERM, safe_quit_process);
    signal(SIGINT, safe_quit_process);

    while (!shouldStop) {
        pcpp::multiPlatformSleep(1);
    }
    config.getMDevice()->stopCapture();
    config.getMDevice()->close();
    for (auto i : filterPointersForDeletion) {
        delete (pcpp::PortFilter *) i;
    }
    delete server;
    std::cout << "quit" << std::endl;

    return 0;
}
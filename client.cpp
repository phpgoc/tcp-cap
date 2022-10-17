#include "Config.h"
#include <iostream>
#include <unistd.h>
using namespace std;
#include "PcapLiveDeviceList.h"
#include "SystemUtils.h"

static bool shouldStop = false;
Config config;

static void on_packet_arrives(pcpp::RawPacket *packet, pcpp::PcapLiveDevice *dev, void *cookie) {
    // extract the stats object form the cookie
    server_driver::Base *server = (server_driver::Base *) cookie;
    // parsed the raw packet
    pcpp::Packet parsedPacket(packet);
    cout << parsedPacket.toString() << endl;
    char *base_ptr = (char *) malloc(sizeof(int) + sizeof(timespec) + sizeof(pcpp::IPAddress) + packet->getRawDataLen());
    char *ptr = base_ptr;
    int *pInt = (int *) ptr;
    *pInt = packet->getRawDataLen();
    ptr += sizeof(int);
    timespec *time = (timespec *) ptr;
    *time = packet->getPacketTimeStamp();
    ptr += sizeof(timespec);
    pcpp::IPAddress *srcIP = (pcpp::IPAddress *) ptr;
    *srcIP = pcpp::IPAddress{config.getMDevice()->getIPv4Address()};
    ptr += sizeof(pcpp::IPAddress);
    memcpy(ptr, packet->getRawData(), packet->getRawDataLen());
    server->push(string(base_ptr, sizeof(int) + sizeof(timespec) + sizeof(pcpp::IPAddress) + packet->getRawDataLen()));
    free(base_ptr);
}
int main(int argc, char *argv[]) {
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

    pcpp::BPFStringFilter tls{" (((tcp[((tcp[12] & 0xf0) >> 2)] = 0x14) || (tcp[((tcp[12] & 0xf0) >> 2)] = 0x15) || (tcp[((tcp[12] & 0xf0) >> 2)] = 0x17)) && (tcp[((tcp[12] & 0xf0) >> 2)+1] = 0x03 && (tcp[((tcp[12] & 0xf0) >> 2)+2] < 0x03)))   ||   (tcp[((tcp[12] & 0xf0) >> 2)] = 0x16) && (tcp[((tcp[12] & 0xf0) >> 2)+1] = 0x03) && (tcp[((tcp[12] & 0xf0) >> 2)+9] = 0x03) && (tcp[((tcp[12] & 0xf0) >> 2)+10] < 0x03)    ||    (((tcp[((tcp[12] & 0xf0) >> 2)] < 0x14) || (tcp[((tcp[12] & 0xf0) >> 2)] > 0x18)) && (tcp[((tcp[12] & 0xf0) >> 2)+3] = 0x00) && (tcp[((tcp[12] & 0xf0) >> 2)+4] = 0x02))"};
    pcpp::NotFilter not_tls(&tls);
    //todo 上边写的tls过滤器有问题,并不成立，还没找到可以过滤tls包的bpf写法
//    and_filter.addFilter(&not_tls);
    string filterAsString;
    and_filter.parseToString(filterAsString);
    cout << "filterAsString: " << filterAsString << endl;

//    return 0;
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
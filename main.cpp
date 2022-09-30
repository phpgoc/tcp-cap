#include "Config.h"
#include "server_driver/Factory.h"
#include <iostream>
#include <unistd.h>
using namespace std;
#include "PcapLiveDeviceList.h"
#include "SystemUtils.h"
static void onPacketArrives(pcpp::RawPacket *packet, pcpp::PcapLiveDevice *dev, void *cookie) {
    // extract the stats object form the cookie

    // parsed the raw packet
    pcpp::Packet parsedPacket(packet);
    cout << parsedPacket.toString() << endl;
    // print packet info
}
int main(int argc, char *argv[]) {
    Config c;
    if (argc > 1) {
        if (string(argv[1]) == "-h") {
            printf("help");
            return 0;
        }
        c.set_config(argv[1]);
    } else {
        c.set_config(nullptr);
    }
    c.debug();
    server_drvier::Base *server = server_drvier::get_server_instance(c.getMServerType(), c.getMServerIp(), c.getMServerPort());
    //    for (int j = 1; j < 5; j++) {
    //        for (int i = 0; i < 1000; i++) {
    //            server->push(to_string(i));
    //        }
    //        sleep(1);
    //    }
    pcpp::OrFilter OrFilter;
    for (auto i : c.getMDbs()) {
        // create a filter instance to capture only traffic on port 80
        pcpp::PortFilter *portFilter = new pcpp::PortFilter(i.first, pcpp::SRC_OR_DST);

        OrFilter.addFilter(portFilter);
    }
    pcpp::AndFilter AndFilter;
    AndFilter.addFilter(&OrFilter);
    pcpp::ProtoFilter protocolFilter(pcpp::TCP);
    AndFilter.addFilter(&protocolFilter);
    c.getMDevice()->setFilter(AndFilter);
    c.getMDevice()->startCapture(onPacketArrives, nullptr);
    pcpp::multiPlatformSleep(2147483647);
    delete server;
    return 0;
}
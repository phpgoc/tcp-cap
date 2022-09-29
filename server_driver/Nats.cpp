//
// Created by 杨殿擎 on 2022/9/29.
//

#include "Nats.h"
#include <iostream>
using namespace std;
server_drvier::Nats::Nats(const std::string &ip, int port) {
    natsOptions *opts;
    string url = "nats://" + ip + ":" + to_string(port);
    natsOptions_SetServers(opts, (const char **) &url, 1);
    status = natsConnection_Connect(&conn, opts);
    if (status != NATS_OK) {
        cerr << "Nats Connetion failed " << endl;
        exit(1);
    }
    jsOptions *jsOpts;
    jsOptions_Init(jsOpts);
    status = natsConnection_JetStream(&js, conn, jsOpts);
}

void server_drvier::Nats::push(const std::string &b) {
    status = js_PublishAsync(js, "message", (const void *) b.c_str(), b.size(), NULL);
    //    natsConnection_Publish(conn, "message", (const void *) b.c_str(), b.size());
}

//
// Created by 杨殿擎 on 2022/9/29.
//

#include "Nats.h"
#include <iostream>
using namespace std;
server_drvier::Nats::Nats(const std::string &ip, int port) {

    string url = "nats://" + ip + ":" + to_string(port);
    natsOptions_SetServers(m_opts, (const char **) &url, 1);
    status = natsConnection_Connect(&conn, m_opts);
    if (status != NATS_OK) {
        cerr << "Nats Connetion failed " << endl;
        exit(1);
    }

    jsOptions_Init(m_jsOpts);
    status = natsConnection_JetStream(&m_js, conn, m_jsOpts);
}
server_drvier::Nats::~Nats() {
    if (m_js)
        delete m_js;
    if (m_jsOpts)
        delete m_jsOpts;
    if (m_jsOpts)
        delete m_opts;
}

void server_drvier::Nats::push(const std::string &b) {
    status = js_PublishAsync(m_js, "message", (const void *) b.c_str(), b.size(), NULL);
    //    natsConnection_Publish(conn, "message", (const void *) b.c_str(), b.size());
}

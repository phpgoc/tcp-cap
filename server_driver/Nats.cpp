//
// Created by 杨殿擎 on 2022/9/29.
//

#include "Nats.h"
#include <iostream>
using namespace std;
server_drvier::Nats::Nats(const std::string &ip, int port, std::string message_queue) : Base(message_queue) {

    string url = "nats://" + ip + ":" + to_string(port);
    natsOptions_SetServers(mp_opts, (const char **) &url, 10);
    m_status = natsConnection_Connect(&m_conn, mp_opts);
    if (m_status != NATS_OK) {
        cerr << "Nats Connetion failed " << endl;
        exit(1);
    }

    jsOptions_Init(mp_jsOpts);
    m_status = natsConnection_JetStream(&mp_js, m_conn, mp_jsOpts);
}
server_drvier::Nats::~Nats() {
    if (mp_js)
        delete mp_js;
    if (mp_jsOpts)
        delete mp_jsOpts;
    if (mp_jsOpts)
        delete mp_opts;
}

void server_drvier::Nats::push(const std::string &b) {

    //    m_status = js_PublishAsync(mp_js, "message", (const void *) b.data(), b.size(), NULL);
    natsConnection_Publish(m_conn, "message", (const void *) b.c_str(), b.size());
}

void server_drvier::Nats::pull_loop(void (*handle)(const std::string &), bool *stop) {
    natsSubscription *sub;
    natsMsg *msg;
    natsStatus s;

    //    s = natsConnection_Subscribe(&sub, m_conn, "message", NULL);
    //    if (s != NATS_OK) {
    //        cerr << "Nats Subscribe failed " << endl;
    //        exit(1);
    //    }
    //    while (true) {
    //        s = natsSubscription_NextMsg(&msg, sub);
    //        if (s != NATS_OK) {
    //            cerr << "Nats Subscribe failed " << endl;
    //            continue;
    //        }
    //        handle(natsMsg_GetData(msg));
    //        natsMsg_Destroy(msg);
    //    }
}

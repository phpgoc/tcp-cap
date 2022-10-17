//
// Created by 杨殿擎 on 2022/9/29.
//

#include "Nats.h"
#include "SystemUtils.h"
#include <iostream>

using namespace std;

static void (*handle_function)(const std::string &);

static void onMsg(natsConnection *nc, natsSubscription *sub, natsMsg *msg, void *closure) {
    handle_function(string(natsMsg_GetData(msg), natsMsg_GetDataLength(msg)));
    natsMsg_AckSync(msg, nullptr, 0);

}
static void asyncCb(natsConnection *nc, natsSubscription *sub, natsStatus err, void *closure) {
    printf("Async error: %u - %s\n", err, natsStatus_GetText(err));
}


server_driver::Nats::Nats(const std::string &ip, int port, const std::string &message_queue, const std::string &stream_name) : Base(message_queue) {
    m_stream_name = stream_name;
    constexpr int MAX_SERVERS = 1;
    char *serverUrls[MAX_SERVERS];
    string url = "nats://" + ip + ":" + to_string(port);
    memset(serverUrls, 0, sizeof(serverUrls));
    serverUrls[0] = (char *) url.c_str();
    natsOptions_Create(&mp_opts);
    m_status = natsOptions_SetErrorHandler(mp_opts, asyncCb, NULL);
    m_status = natsOptions_SetServers(mp_opts, (const char **) serverUrls, MAX_SERVERS);
    if (m_status != NATS_OK) {
        nats_PrintLastErrorStack(stderr);
        exit(1);
    }
    m_status = natsOptions_SetReconnectWait(mp_opts, 100);
    // Instruct the library to block the connect call for that
    // long until it can get a connection or fails.
    if (m_status == NATS_OK) {
        m_status = natsOptions_SetRetryOnFailedConnect(mp_opts, true, nullptr, nullptr);
    }
    m_status = natsConnection_Connect(&m_conn, mp_opts);
    cout << "nats connect status: " << natsStatus_GetText(m_status) << endl;
    if (m_status != NATS_OK || natsConnection_IsClosed(m_conn)) {
        cerr << "Nats Connetion failed " << endl;
        exit(1);
    }

    if (m_status == NATS_OK)
        m_status = jsOptions_Init(&m_jsOpts);

    if (m_status == NATS_OK) {
        m_status = natsConnection_JetStream(&mp_js, m_conn, &m_jsOpts);
    }


    m_status = js_GetStreamInfo(&mp_si, mp_js, m_stream_name.c_str(), NULL, nullptr);
    if (m_status == NATS_NOT_FOUND) {
        cout << "stream not found, create stream:" << m_stream_name<< endl;
        if (m_stream_name != "default") {
            cerr << "Stream " << m_stream_name << " does not exist" << endl;
            cerr << "You can set it to default for test" << endl;
            exit(1);
        }
        jsStreamConfig cfg;

        // Initialize the configuration structure.
        jsStreamConfig_Init(&cfg);
        cfg.Name = m_stream_name.c_str();
        // Set the subject
        cfg.Subjects = (const char *[1]){this->getMessageQueue().c_str()};
        cfg.SubjectsLen = 1;
        // Make it a memory mp_stream_name.
        cfg.Retention = js_WorkQueuePolicy;
//        cfg.DenyDelete = false;
        cfg.Storage = js_MemoryStorage;
        // Add the mp_stream_name,
        m_status = js_AddStream(&mp_si, mp_js, &cfg, NULL, nullptr);
    }
}

server_driver::Nats::~Nats() {
    if (mp_opts) {
        natsOptions_Destroy(mp_opts);
    }
    natsConnection_Destroy(m_conn);
    nats_Close();
}

void server_driver::Nats::push(const std::string &b) {

    m_status = js_PublishAsync( mp_js, this->getMessageQueue().c_str(), (const void *) b.data(), b.size(), nullptr);
    cout << "nats push status: " << natsStatus_GetText(m_status) << endl;
    //    natsConnection_Publish(m_conn, this->getMessageQueue().c_str(), (const void *) b.c_str(), b.size());
}


void server_driver::Nats::pull_loop(void (*handle)(const std::string &), bool *stop) {
    natsSubscription *sub;
    jsSubOptions so;
    m_status = jsSubOptions_Init(&so);
    if (m_status == NATS_OK) {
        m_status = jsSubOptions_Init(&so);
    }
    //    if (m_status == NATS_OK) {
    //        so.Stream = m_stream_name.c_str();
    //        //        so.Consumer = durable;
    //        so.Config.FlowControl = false;
    //        so.Config.Heartbeat = (int64_t) 1E9;
    //    }
    handle_function = handle;


    m_status = js_Subscribe(&sub, mp_js, this->getMessageQueue().c_str(), onMsg, nullptr, &m_jsOpts, &so, nullptr);

    if (m_status == NATS_OK)
        m_status = natsSubscription_SetPendingLimits(sub, -1, -1);
    while (!*stop) {
        pcpp::multiPlatformSleep(1);
    }
}

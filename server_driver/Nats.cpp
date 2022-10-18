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
    natsOptions_SetMaxReconnect(mp_opts, -1);
    natsOptions_SetReconnectWait(mp_opts, 100);
    m_status = natsOptions_SetErrorHandler(mp_opts, asyncCb, nullptr);

    m_status = natsOptions_SetServers(mp_opts, (const char **) serverUrls, MAX_SERVERS);
    if (m_status != NATS_OK) {
        nats_PrintLastErrorStack(stderr);
        exit(1);
    }

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


    m_status = js_GetStreamInfo(&mp_si, mp_js, m_stream_name.c_str(), nullptr, nullptr);
    if (m_status == NATS_NOT_FOUND) {
        cout << "stream not found, create stream:" << m_stream_name << endl;
        if (m_stream_name != "default") {
            cerr << "Stream " << m_stream_name << " does not exist" << endl;
            cerr << "You can set it to default for test" << endl;
            exit(1);
        }
        jsStreamConfig cfg;

        jsStreamConfig_Init(&cfg);
        cfg.Name = m_stream_name.c_str();
        cfg.Subjects = (const char *[1]){this->getMessageQueue().c_str()};
        cfg.SubjectsLen = 1;
        cfg.Retention = js_WorkQueuePolicy;//收到消费端ack删除消息
        cfg.Storage = js_MemoryStorage;
        m_status = js_AddStream(&mp_si, mp_js, &cfg, nullptr, nullptr);
    }
}

server_driver::Nats::~Nats() {

    natsConnection_Destroy(m_conn);
    nats_Close();
    if (mp_opts) {
        natsOptions_Destroy(mp_opts);
    }
}

void server_driver::Nats::push(const std::string &b) {

    m_status = js_PublishAsync(mp_js, this->getMessageQueue().c_str(), (const void *) b.data(), b.size(), nullptr);
    if (m_status != NATS_OK) {
        nats_PrintLastErrorStack(stderr);
    }
}


void server_driver::Nats::pull_loop(void (*handle)(const std::string &), bool *stop) {
    handle_function = handle;

    natsSubscription *sub;
    jsSubOptions so;
    m_status = jsSubOptions_Init(&so);

//    so.Config.FlowControl = true;       //开启流控
//    so.Config.Heartbeat = (int64_t) 1E9;//参数是纳秒 ,1秒拉一次
    //todo 未来可能会改拉取模式，目前的方式流量不可控。很快，但不稳，OnMessage如果完全异步，即使client发送的消息对，server组装也可能会丢消息（加锁）。
    m_status = js_Subscribe(&sub, mp_js, this->getMessageQueue().c_str(), onMsg, nullptr, &m_jsOpts, &so, nullptr);

    if (m_status == NATS_OK){
        m_status = natsSubscription_SetPendingLimits(sub, -1, -1);
    }
    while (!*stop) {
        pcpp::multiPlatformSleep(1);
    }
}

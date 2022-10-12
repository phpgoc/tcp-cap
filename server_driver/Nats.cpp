//
// Created by 杨殿擎 on 2022/9/29.
//

#include "Nats.h"
#include "nats/adapters/libuv.h"
#include "SystemUtils.h"
#include <iostream>

using namespace std;

static void (*handle_function)(const std::string &);

static void onMsg(natsConnection *nc, natsSubscription *sub, natsMsg *msg, void *closure) {
    handle_function(string(natsMsg_GetData(msg), natsMsg_GetDataLength(msg)));
}

server_drvier::Nats::Nats(const std::string &ip, int port, std::string message_queue) : Base(message_queue) {
    constexpr int MAX_SERVERS = 1;
    char *serverUrls[MAX_SERVERS];
    string url = "nats://" + ip + ":" + to_string(port);
    natsOptions_Create(&mp_opts);

    memset(serverUrls, 0, sizeof(serverUrls));
    serverUrls[0] = (char *) url.c_str();
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
}

server_drvier::Nats::~Nats() {
    if (mp_opts) {
        natsOptions_Destroy(mp_opts);
    }
    natsConnection_Destroy(m_conn);
    nats_Close();

}

void server_drvier::Nats::push(const std::string &b) {

    //    m_status = js_PublishAsync(mp_js, "message", (const void *) b.data(), b.size(), nullptr);
    natsConnection_Publish(m_conn, this->getMessageQueue().c_str(), (const void *) b.c_str(), b.size());
}

static void asyncCb(natsConnection *nc, natsSubscription *sub, natsStatus err, void *closure) {
    printf("Async error: %u - %s\n", err, natsStatus_GetText(err));
}


void server_drvier::Nats::pull_loop(void (*handle)(const std::string &), bool *stop) {
    natsSubscription *sub;

    handle_function = handle;
    natsLibuv_Init();
    uv_loop_t *uvLoop = nullptr;
    // Create a loop.
    uvLoop = uv_default_loop();
    m_status = natsOptions_SetErrorHandler(mp_opts, asyncCb, NULL);
    if (uvLoop != nullptr) {
        // Libuv is not thread-safe. Almost all calls to libuv need to
        // occur from the thread where the loop is running. NATS library
        // may have to call into the event loop from different threads.
        // This call allows natsLibuv APIs to know if they are executing
        // from the event loop thread or not.
        natsLibuv_SetThreadLocalLoop(uvLoop);
    } else {
        m_status = NATS_ERR;
    }

    // Indicate which loop and callbacks to use once connected.
    if (m_status == NATS_OK) {
        m_status = natsOptions_SetEventLoop(mp_opts, (void *) uvLoop,
                                            natsLibuv_Attach,
                                            natsLibuv_Read,
                                            natsLibuv_Write,
                                            natsLibuv_Detach);
    }

    if (m_status == NATS_OK) {
        m_status = natsConnection_Subscribe(&sub, m_conn, this->getMessageQueue().c_str(), onMsg, nullptr);

    }
    // For maximum performance, set no limit on the number of pending messages.
    if (m_status == NATS_OK) {
        m_status = natsSubscription_SetPendingLimits(sub, -1, -1);
    }

    while (!*stop) {
        pcpp::multiPlatformSleep(1);
//        uv_run(uvLoop, UV_RUN_ONCE);
    }

    natsSubscription_Destroy(sub);

    if (uvLoop != nullptr)
        uv_loop_close(uvLoop);

}

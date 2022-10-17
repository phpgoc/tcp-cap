//
// Created by 杨殿擎 on 2022/9/29.
//

#ifndef VCPKG_1_NATS_H
#define VCPKG_1_NATS_H
#include "Base.h"
#include <nats/nats.h>
namespace server_driver {
    class Nats : public Base {
    public:
        Nats(const  std::string &ip, int port,  const std::string &message_queue, const  std::string &stream_name);
        ~Nats();
        void push(const std::string &b) override;
        void pull_loop(void (*handle)(const std::string &), bool *stop) override;

    private:
        natsConnection *m_conn;
        natsStatus m_status;
        natsOptions *mp_opts;
        jsCtx *mp_js = nullptr;
        std::string m_stream_name;
        jsOptions m_jsOpts;
        jsStreamInfo *mp_si = nullptr;
    };

}// namespace server_driver
#endif//VCPKG_1_NATS_H

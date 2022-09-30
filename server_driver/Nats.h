//
// Created by 杨殿擎 on 2022/9/29.
//

#ifndef VCPKG_1_NATS_H
#define VCPKG_1_NATS_H
#include "Base.h"
#include <nats/nats.h>
namespace server_drvier {
    class Nats : public Base {
    public:
        Nats(const std::string &ip, int port, std::string message_queue);
        ~Nats();
        void push(const std::string &b) override;
        void pull_loop(void (*handle)(const std::string &)) override;

    private:
        natsConnection *m_conn;
        natsStatus m_status;
        jsCtx *m_p_js;
        natsOptions *m_p_opts;
        jsOptions *m_p_jsOpts;
    };

}// namespace server_drvier
#endif//VCPKG_1_NATS_H

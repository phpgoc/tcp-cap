//
// Created by 杨殿擎 on 2022/9/29.
//

#ifndef VCPKG_1_NATS_H
#define VCPKG_1_NATS_H
#include "Base.h"
#include <nats.h>
namespace server_drvier {
    class Nats : public Base {
    public:
        Nats(const std::string &ip, int port);
        ~Nats();
        void push(const std::string &b) override;

    private:
        natsConnection *conn;
        natsStatus status;
        jsCtx *js;
    };

}// namespace server_drvier
#endif//VCPKG_1_NATS_H

//
// Created by 杨殿擎 on 2022/9/29.
//

#ifndef VCPKG_1_BASE_H
#define VCPKG_1_BASE_H
#include <string>
namespace server_driver {

    class Base {
    public:
        Base(const std::string queue) : message_queue(queue) {}
        virtual void push(const std::string &b) = 0;
        virtual void pull_loop(void (*handle)(const std::string &),bool *stop) = 0;
        virtual ~Base() {}


    protected:
        const std::string &getMessageQueue() const {
            return message_queue;
        }
        std::string message_queue;
    };
}// namespace server_driver

#endif//VCPKG_1_BASE_H

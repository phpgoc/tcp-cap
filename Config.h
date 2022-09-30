//
// Created by 杨殿擎 on 2022/9/29.
//

#ifndef VCPKG_1_CONFIG_H
#define VCPKG_1_CONFIG_H
#include "PcapLiveDeviceList.h"
#include "SystemUtils.h"
#include <string>
#include <unordered_map>
struct machine {
    std::string type;
    int port;
};
class Config {
public:
    void debug();
    Config();
    void set_config(char *str);
    const std::string &getMServerType() const;
    const std::string &getMServerIp() const;
    int getMServerPort() const;
    const std::unordered_map<int, std::string> &getMDbs() const;
    pcpp::PcapLiveDevice *getMDevice() const;
    const std::string &getMMessageQueue() const;

private:
    std::string m_config_file = "config.toml";
    std::string m_server_type;
    std::string m_server_ip;
    int m_server_port;
    std::string m_message_queue;
    std::unordered_map<int, std::string> m_dbs;
    pcpp::PcapLiveDevice *m_device;
};


#endif//VCPKG_1_CONFIG_H

//
// Created by 杨殿擎 on 2022/9/30.
//

#ifndef VCPKG_1_SERVERCONFIG_H
#define VCPKG_1_SERVERCONFIG_H
#include <string>

class ServerConfig {
public:
    ServerConfig();
    const std::string &getMServerType() const;
    const std::string &getMServerIp() const;
    int getMServerPort() const;
    void debug();
    const std::string &getMMessageQueue() const;

    void set_config(char *string);

private:
    std::string m_config_file = "server_config.toml";

    std::string m_server_type;
    std::string m_server_ip;
    int m_server_port;
    std::string m_message_queue;
};


#endif//VCPKG_1_SERVERCONFIG_H

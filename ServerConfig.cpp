//
// Created by 杨殿擎 on 2022/9/30.
//
#include "ServerConfig.h"
#include "tools.h"
#include <iostream>
#include <toml++/toml.h>
using namespace std;
ServerConfig::ServerConfig() {
}
void ServerConfig::debug() {
    cout << "server_type: " << m_server_type << endl;
    cout << "server_ip: " << m_server_ip << endl;
    cout << "server_port: " << m_server_port << endl;
}

const string &ServerConfig::getMServerType() const {
    return m_server_type;
}
const string &ServerConfig::getMServerIp() const {
    return m_server_ip;
}
int ServerConfig::getMServerPort() const {
    return m_server_port;
}
const string &ServerConfig::getMMessageQueue() const {
    return m_message_queue;
}
void ServerConfig::set_config(char *string) {
    if (string != nullptr) {
        m_config_file = string;
    }
    toml::table config;
    try {
        config = toml::parse_file(m_config_file);
    } catch (const toml::parse_error &err) {
        cerr << "Parsing failed:\n"
             << err << "\n";
        exit(1);
    }
    // server segment
    m_server_type = config["message_server"]["type"].value_or("");
    check_server_type_valid(m_config_file, m_server_type);
    m_server_ip = config["message_server"]["ip"].value_or("");
    check_server_ip_valid(m_config_file, &m_server_ip);
    m_server_port = config["message_server"]["port"].value_or(0);
    check_server_port_valid(m_config_file, m_server_port);
    m_message_queue = config["message_server"]["queue"].value_or("");
    check_server_queue_valid(m_config_file, m_message_queue);
}

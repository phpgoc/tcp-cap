//
// Created by 杨殿擎 on 2022/9/30.
//
#include "ServerConfig.h"
#include "tools.h"
#include <iostream>
#include <toml++/toml.h>
using namespace std;
string config_file = "server_config.toml";
ServerConfig::ServerConfig() {
    toml::table config;
    try {
        config = toml::parse_file(config_file);
    } catch (const toml::parse_error &err) {
        cerr << "Parsing failed:\n"
             << err << "\n";
        exit(1);
    }
    // server segment
    m_server_type = config["message_server"]["type"].value_or("");
    check_server_type_valid(config_file, m_server_type);
    m_server_ip = config["message_server"]["ip"].value_or("");
    check_server_ip_valid(config_file, &m_server_ip);
    m_server_port = config["message_server"]["port"].value_or(0);
    check_server_port_valid(config_file, m_server_port);
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

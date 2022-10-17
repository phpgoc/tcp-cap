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
void ServerConfig::info() {
    cout << "server_type: " << m_server_type << endl
         << "server_ip: " << m_server_ip << endl
         << "server_port: " << m_server_port << endl
         << "message_queue: " << m_message_queue << endl
         << "stream_name: " << m_stream_name << endl;
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
    if (m_server_type == "nats") {
        m_stream_name = config["nats"]["stream"].value_or("");
        if (m_stream_name.empty()) {
            m_stream_name = "default";
        }
    }
    m_server_ip = config["message_server"]["ip"].value_or("");
    check_server_ip_valid(m_config_file, &m_server_ip);
    m_server_port = config["message_server"]["port"].value_or(0);
    check_server_port_valid(m_config_file, m_server_port);
    m_message_queue = config["message_server"]["queue"].value_or("");
    check_server_queue_valid(m_config_file, m_message_queue);
}
const string &ServerConfig::getMStreamName() const {
    return m_stream_name;
}

server_driver::Base *get_message_server_instance( ServerConfig &config) {
    if(config.getMServerType() == "redis") {
        return new server_driver::Redis(config.getMServerIp(), config.getMServerPort(), config.getMMessageQueue());
    } else if(config.getMServerType() == "nats") {
        return new server_driver::Nats(config.getMServerIp(), config.getMServerPort(), config.getMMessageQueue(), config.getMStreamName());
    }
    cerr << "not support type: " << config.getMServerType() << endl;
    cerr << "support type: redis, nats" << endl;
    exit(1);
}


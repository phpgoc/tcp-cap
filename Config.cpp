//
// Created by 杨殿擎 on 2022/9/29.
//

#include "Config.h"
#include "tools.h"
#include <iostream>
#include <toml++/toml.h>
using namespace std;


Config::Config() {
}
void Config::set_config(char *str) {
    if (str != nullptr) {
        m_config_file = str;
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
    // device segment
    string device_ip = config["device"]["ip"].value_or("");
    string device_name = config["device"]["name"].value_or("");
    if (device_ip.empty() && device_ip.empty()) {
        cerr << "At least one IP address and one name must be set in the device segment"
             << "\n";
        exit(1);
    }
    m_device = pcpp::PcapLiveDeviceList::getInstance().getPcapLiveDeviceByIp(
            device_ip);
    if (!m_device) {
        m_device = pcpp::PcapLiveDeviceList::getInstance().getPcapLiveDeviceByName(device_name);
    }
    if (!m_device) {
        cerr << "Wrong device config";
        exit(1);
    }
    if (!m_device->open()) {
        std::cerr << "Cannot open device" << std::endl;
        exit(1);
    }
    if (auto dbs = config["db"]; dbs) {
        for (auto &db : *dbs.as_array()) {
            if (!db.as_table()->contains("port") || !db.as_table()->contains("type")) {
                cerr << "Parsing failed:" << m_config_file << ":db.port or db.type is invalid"
                     << "\n";
                exit(1);
            }
            auto type = db.as_table()->at("type").value<string>();
            auto port = db.as_table()->at("port").value<int>();
            if (m_dbs.contains(*port)) {
                cerr << "Parsing failed:" << m_config_file << ":db.port is duplicate"
                     << "\n";
                exit(1);
            }
            m_dbs[*port] = *type;
        }
    } else {
        cerr << "Parsing failed: " << m_config_file << ": The db array segment must exist";
        exit(1);
    }
}

void Config::debug() {
    cout << "Server IP: " << m_server_ip << endl
         << "Server type: " << m_server_type << endl
         << "Server port: " << m_server_port << endl;

    for (auto v : m_dbs) {
        cout << "DB port: " << v.first << " DB type: " << v.second << endl;
    }
}
const string &Config::getMServerType() const {
    return m_server_type;
}
const string &Config::getMServerIp() const {
    return m_server_ip;
}
int Config::getMServerPort() const {
    return m_server_port;
}
const unordered_map<int, std::string> &Config::getMDbs() const {
    return m_dbs;
}
pcpp::PcapLiveDevice *Config::getMDevice() const {
    return m_device;
}
const string &Config::getMMessageQueue() const {
    return m_message_queue;
}

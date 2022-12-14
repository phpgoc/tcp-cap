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
    if(m_server_type == "nats"){
            m_stream_name = config["nats"]["stream"].value_or("");
            if(m_stream_name.empty()){
               m_stream_name = "default";
            }
    }
    m_server_ip = config["message_server"]["ip"].value_or("");
    check_server_ip_valid(m_config_file, &m_server_ip);
    m_server_port = config["message_server"]["port"].value_or(0);
    check_server_port_valid(m_config_file, m_server_port);
    m_message_queue = config["message_server"]["queue"].value_or("");
    check_server_queue_valid(m_config_file, m_message_queue);
    // device segment
    string device_ip = config["device"]["ip"].value_or("");
    string device_name = config["device"]["name"].value_or("");
    if (device_ip.empty() && device_name.empty()) {
        cerr << "At least one IP address and one name must be set in the device segment"
             << "\n";
        exit(1);
    }
    mp_device = pcpp::PcapLiveDeviceList::getInstance().getPcapLiveDeviceByIp(
            device_ip);
    if (!mp_device) {
        mp_device = pcpp::PcapLiveDeviceList::getInstance().getPcapLiveDeviceByName(device_name);
    }
    if (!mp_device) {
        cerr << "Wrong device config";
        exit(1);
    }
    if (!mp_device->open()) {
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

void Config::info() {
    cout << "Device name: " << mp_device->getName() << endl
         << "Device IP: " << mp_device->getIPv4Address().toString() << endl
         << "Server IP: " << m_server_ip << endl
         << "Server type: " << m_server_type << endl
         << "Server port: " << m_server_port << endl
         << "Server IP: " << m_server_ip << endl
         << "Server type: " << m_server_type << endl
         << "message_queue: " << m_message_queue << endl
            << "stream_name: " << m_stream_name << endl;
    for (auto db : m_dbs) {
        cout << "DB port: " << db.first << " DB type: " << db.second << endl;
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
    return mp_device;
}
const string &Config::getMMessageQueue() const {
    return m_message_queue;
}
const string &Config::getMStreamName() const {
    return m_stream_name;
}

server_driver::Base *get_message_server_instance( Config &config) {
    if(config.getMServerType() == "redis") {
        return new server_driver::Redis(config.getMServerIp(), config.getMServerPort(), config.getMMessageQueue());
    } else if(config.getMServerType() == "nats") {
        return new server_driver::Nats(config.getMServerIp(), config.getMServerPort(), config.getMMessageQueue(), config.getMStreamName());
    }
    cerr << "not support type: " << config.getMServerType() << endl;
    cerr << "support type: redis, nats" << endl;
    exit(1);
}


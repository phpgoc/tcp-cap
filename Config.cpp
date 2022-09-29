//
// Created by 杨殿擎 on 2022/9/29.
//

#include "Config.h"
#include <iostream>
#include <toml++/toml.h>
using namespace std;
string config_file = "Config.toml";
static inline void check_server_type_valid(const string &server_type) {
    if (server_type.empty()) {
        cerr << "Parsing failed:" << config_file << ":server.type is invalid"
             << "\n";
        exit(1);
    }
}

static inline void check_server_ip_valid(const string *server_type) {

    if (server_type->empty()) {
        cerr << "Parsing failed:" << config_file << ":server.ip is invalid"
             << "\n";
        exit(1);
    }
}

static inline void check_server_port_valid(int server_port) {
    if (0 == server_port) {
        cerr << "Parsing failed: " << config_file << ":server.port is invalid"
             << "\n";
        exit(1);
    }
}

Config::Config() {
}
void Config::set_config(char *str) {
    if (str != nullptr) {
        config_file = str;
    }
    toml::table config;
    try {
        config = toml::parse_file(config_file);
    } catch (const toml::parse_error &err) {
        cerr << "Parsing failed:\n"
             << err << "\n";
        exit(1);
    }
    m_server_type = config["server"]["type"].value_or("");
    check_server_type_valid(m_server_type);

    m_server_ip = config["server"]["ip"].value_or("");
    check_server_ip_valid(&m_server_ip);
    m_server_port = config["server"]["port"].value_or(0);
    check_server_port_valid(m_server_port);
    if (auto dbs = config["db"]; dbs) {
        for (auto &db : *dbs.as_array()) {
            if (!db.as_table()->contains("port") || !db.as_table()->contains("type")) {
                cerr << "Parsing failed:" << config_file << ":db.port or db.type is invalid"
                     << "\n";
                exit(1);
            }
            auto type = db.as_table()->at("type").value<string>();
            auto port = db.as_table()->at("port").value<int>();
            if (m_dbs.contains(*port)) {
                cerr << "Parsing failed:" << config_file << ":db.port is duplicate"
                     << "\n";
                exit(1);
            }
            m_dbs[*port] = *type;
        }
    } else {
        cerr << "Parsing failed: " << config_file << ": The db array segment must exist";
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

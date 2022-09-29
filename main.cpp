#include "Config.h"
#include "server_driver/Factory.h"
#include <iostream>
#include <unistd.h>
using namespace std;
int main(int argc, char *argv[]) {
    Config c;
    if (argc > 1) {
        if (string(argv[1]) == "-h") {
            printf("help");
            return 0;
        }
        c.set_config(argv[1]);
    } else {
        c.set_config(nullptr);
    }
    c.debug();
    server_drvier::Base *server = server_drvier::get_server_instance(c.getMServerType(), c.getMServerIp(), c.getMServerPort());
    for (int j = 1; j < 5; j++) {
        for (int i = 0; i < 10000; i++) {
            server->push(to_string(i));
        }
        sleep(j);
    }

    delete server;
    return 0;
}
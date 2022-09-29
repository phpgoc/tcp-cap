#include "Config.h"
#include <iostream>
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
    return 0;
}
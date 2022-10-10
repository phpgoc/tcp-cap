//
// Created by 杨殿擎 on 2022/10/10.
//

#ifndef DATA_INGEST_ENTRANCE_H
#define DATA_INGEST_ENTRANCE_H

#include <iostream>
namespace protocol {
    class Entrance {
    public:
        static void handle(const std::string &b);
        Entrance();
        ~Entrance();

    };
}// namespace protocol



#endif//DATA_INGEST_ENTRANCE_H

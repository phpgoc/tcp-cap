//
// Created by 杨殿擎 on 2022/10/8.
//

#ifndef DATA_INGEST_DELETEQUEUE_H
#define DATA_INGEST_DELETEQUEUE_H

#include <queue>
#include <IpAddress.h>
namespace tcp_reassembly {
    class DeleteQueue{
    public :
        DeleteQueue(uint32_t i, timeval timeval1, pcpp::IPAddress address);
        bool operator<(const DeleteQueue &rhs) const;

    private:
        uint32_t flow_key;
        timeval end_time;
        pcpp::IPAddress dst_ip;
    };
    static std::priority_queue<tcp_reassembly::DeleteQueue> delete_queue;

    void delete_queue_thread();
    void add_2_delete_queue(uint32_t flow_key, timeval end_time, pcpp::IPAddress dst_ip);
}



#endif//DATA_INGEST_DELETEQUEUE_H

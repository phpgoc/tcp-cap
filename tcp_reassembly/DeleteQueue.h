//
// Created by 杨殿擎 on 2022/10/8.
//

#ifndef DATA_INGEST_DELETEQUEUE_H
#define DATA_INGEST_DELETEQUEUE_H

#include <IpAddress.h>
#include <queue>
namespace tcp_reassembly {
    class DeleteQueue {
    public:
        DeleteQueue(uint32_t i, timeval timeval1, pcpp::IPAddress address);
        bool operator<(const DeleteQueue &rhs) const;

        uint32_t getMFlowKey() const;
        const timeval &getMEndTime() const;
        const pcpp::IPAddress &getMDstIp() const;

    private:
        uint32_t m_flow_key;
        timeval m_end_time;
        pcpp::IPAddress m_dst_ip;
    };
    static std::priority_queue<tcp_reassembly::DeleteQueue> delete_queue;

    void delete_queue_thread(bool *stop);
    void add_2_delete_queue(uint32_t flow_key, timeval end_time, pcpp::IPAddress dst_ip);
}// namespace tcp_reassembly


#endif//DATA_INGEST_DELETEQUEUE_H

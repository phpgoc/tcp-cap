//
// Created by 杨殿擎 on 2022/10/8.
//

#include "DeleteQueue.h"

bool tcp_reassembly::DeleteQueue::operator<(const tcp_reassembly::DeleteQueue &rhs) const {
    return end_time.tv_sec < rhs.end_time.tv_sec;
}
tcp_reassembly::DeleteQueue::DeleteQueue(uint32_t i, timeval timeval1, pcpp::IPAddress address) {
    flow_key = i;
    end_time = timeval1;
    dst_ip = address;
}

void tcp_reassembly::delete_queue_thread() {

}

void tcp_reassembly::add_2_delete_queue(uint32_t flow_key, timeval end_time, pcpp::IPAddress dst_ip) {
    delete_queue.push(DeleteQueue{flow_key, end_time, dst_ip});
}
//
// Created by 杨殿擎 on 2022/10/8.
//

#include "DeleteQueue.h"
#include "../protocol/Entrance.h"
#include "Reassembly.h"
#include "SystemUtils.h"
#include <sys/time.h>
#include <thread>

using namespace std;

bool tcp_reassembly::DeleteQueue::operator<(const tcp_reassembly::DeleteQueue &rhs) const {
    return m_end_time.tv_sec > rhs.m_end_time.tv_sec;
}

tcp_reassembly::DeleteQueue::DeleteQueue(uint32_t i, timeval timeval1, pcpp::IPAddress address) {
    m_flow_key = i;
    m_end_time = timeval1;
    m_dst_ip = address;
}

uint32_t tcp_reassembly::DeleteQueue::getMFlowKey() const {
    return m_flow_key;
}

const timeval &tcp_reassembly::DeleteQueue::getMEndTime() const {
    return m_end_time;
}

const pcpp::IPAddress &tcp_reassembly::DeleteQueue::getMDstIp() const {
    return m_dst_ip;
}

void tcp_reassembly::delete_queue_thread(bool *stop) {
    thread([=]() {
        DeleteQueue p = DeleteQueue(0, timeval(), pcpp::IPAddress());
        timeval now;
        // 不需要设置特别长，tcp链接及时被delete_queue清理，也不影响正常逻辑。
        //delete_queue_thread 和 connectionEndCallback 都可以处理message。
        constexpr int compatibility_tcp_max_time = 120;

        // 先睡眠一段时间，再开始清理。
        // 避免 当队列队列存在缓冲区时，插入到delete_queue里的数据被立刻清理。
        pcpp::multiPlatformSleep(compatibility_tcp_max_time);

        while (!*stop) {
            if (!delete_queue.empty()) {
                gettimeofday(&now, nullptr);
                p = delete_queue.top();

                long next_sleep_time = p.getMEndTime().tv_sec + compatibility_tcp_max_time - now.tv_sec;
                if (next_sleep_time < 0) {
                    //二次比对，tcp connection 的end_time在不断变化
                    AssemblyData *data = Reassembly::getInstance()->get_data_pointer_from_flow_map(p.getMDstIp(),
                                                                                                   p.getMFlowKey());
                    if (data == nullptr) {
                        //已经被end connection正确处理的。不需要再次处理
                        delete_queue.pop();
                        continue;
                    }
                    next_sleep_time = data->getMEndTime().tv_sec + compatibility_tcp_max_time - now.tv_sec;
                    if (next_sleep_time < 0) {

                        auto map = Reassembly::getInstance()->getAssemblyDataByIP(p.getMDstIp());
                        delete_queue.pop();
                        if (Reassembly::find_in_flow_map_handle_earse_with_lock(map, p.getMFlowKey())) {
                            Reassembly::getInstance()->incr_error_count();
//                            cout << "delete queue size: " << delete_queue.size() << ". ";
                        }

                    } else {
                        //存在delete_queue 里的时间已过期，但是真正的tcp end_time已更新，所以不需要删除，更新下时间放回
                        delete_queue.pop();
                        delete_queue.push(DeleteQueue(p.getMFlowKey(), data->getMEndTime(), p.getMDstIp()));
                    }
                    //不需要sleep
                } else {
                    pcpp::multiPlatformSleep(next_sleep_time);
                }
            } else {
                pcpp::multiPlatformSleep(compatibility_tcp_max_time);
            }
        }
    }).detach();
}

void tcp_reassembly::add_2_delete_queue(uint32_t flow_key, timeval end_time, pcpp::IPAddress dst_ip) {
    delete_queue.push(DeleteQueue{flow_key, end_time, dst_ip});
}
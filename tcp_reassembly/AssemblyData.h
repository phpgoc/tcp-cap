//
// Created by 杨殿擎 on 2022/10/8.
//

#ifndef DATA_INGEST_ASSEMBLYDATA_H
#define DATA_INGEST_ASSEMBLYDATA_H

#include <string>
namespace tcp_reassembly {
    class AssemblyData {
    public:
        AssemblyData(timeval endTime);
        std::string &getMData();
        void setMData(const std::string &mData);
        const timeval &getMEndTime() const;
        void setMEndTime(const timeval &endTime);


    private:
        std::string m_data;
        timeval m_end_time;
    };

}// namespace tcp_reassembly


#endif//DATA_INGEST_ASSEMBLYDATA_H

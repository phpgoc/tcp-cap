//
// Created by 杨殿擎 on 2022/10/8.
//

#ifndef DATA_INGEST_ASSEMBLYDATA_H
#define DATA_INGEST_ASSEMBLYDATA_H

#include <string>
namespace tcp_reassembly{
    class AssemblyData {
    public:
        AssemblyData();
         std::string &getMData() ;
        void setMData(const std::string &mData);
        int getSide() const;
        void setSide(int side);

    private:
            std::string m_data;
        int side;

    };

}



#endif//DATA_INGEST_ASSEMBLYDATA_H

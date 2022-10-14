cmake_minimum_required(VERSION 3.24)
project(data-ingest)
set(client client1)
set(server server1)

set(CMAKE_CXX_STANDARD 20)

include(FindPkgConfig)
pkg_search_module(PCAPPLUSPLUS REQUIRED PcapPlusPlus)

find_package(tomlplusplus CONFIG REQUIRED)
find_package(redis++ CONFIG REQUIRED)
find_package(cnats CONFIG REQUIRED)
find_package(libuv CONFIG REQUIRED)
find_package(libmysql REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}   -L/opt/homebrew/Cellar/pcapplusplus/22.05/lib -lPcap++ -lPacket++ -lCommon++ -lpcap -lpthread -framework SystemConfiguration -framework CoreFoundation")

add_executable(${client} client.cpp Config.cpp Config.h tools.h server_driver/Base.cpp server_driver/Base.h server_driver/Redis.cpp server_driver/Redis.h server_driver/Factory.cpp server_driver/Factory.h server_driver/Nats.cpp server_driver/Nats.h)
target_link_libraries(${client} PRIVATE tomlplusplus::tomlplusplus)
target_link_libraries(${client} PRIVATE redis++::redis++_static)
target_link_libraries(${client} PRIVATE cnats::nats_static)
target_link_libraries(${client} PRIVATE $<IF:$<TARGET_EXISTS:uv_a>,uv_a,uv>)



add_executable(${server} server.cpp tools.h server_driver/Base.cpp server_driver/Base.h server_driver/Redis.cpp server_driver/Redis.h server_driver/Factory.cpp server_driver/Factory.h server_driver/Nats.cpp server_driver/Nats.h ServerConfig.cpp ServerConfig.h tcp_reassembly/functions.cpp tcp_reassembly/functions.h tcp_reassembly/DeleteQueue.cpp tcp_reassembly/DeleteQueue.h tcp_reassembly/Reassembly.cpp tcp_reassembly/Reassembly.h tcp_reassembly/AssemblyData.cpp tcp_reassembly/AssemblyData.h protocol/Entrance.cpp protocol/Entrance.h)
target_link_libraries(${server} PRIVATE tomlplusplus::tomlplusplus)
target_link_libraries(${server} PRIVATE redis++::redis++_static)
target_link_libraries(${server} PRIVATE cnats::nats_static)
target_link_libraries(${server} PRIVATE $<IF:$<TARGET_EXISTS:uv_a>,uv_a,uv>)
target_link_libraries(${server} PRIVATE ${MYSQL_LIBRARIES})



#target_include_directories(${client} PUBLIC ${PCAPPLUSPLUS_INCLUDE_DIRS})
#target_include_directories(${server} PUBLIC ${PCAPPLUSPLUS_INCLUDE_DIRS})
#target_include_directories(${client} PUBLIC ${LIBUV_INCLUDE_DIRS})

cmake_minimum_required(VERSION 3.24)
project(data-ingest)
set(client client1)
set(server server1)

set(CMAKE_CXX_STANDARD 20)

find_package(tomlplusplus CONFIG REQUIRED)
find_package(redis++ CONFIG REQUIRED)
find_package(cnats CONFIG REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}   -L/opt/homebrew/Cellar/pcapplusplus/22.05/lib -lPcap++ -lPacket++ -lCommon++ -lpcap -lpthread -framework SystemConfiguration -framework CoreFoundation")

set(common_file)
add_executable(${client} client.cpp Config.cpp Config.h tools.h server_driver/Base.cpp server_driver/Base.h server_driver/Redis.cpp server_driver/Redis.h server_driver/Factory.cpp server_driver/Factory.h server_driver/Nats.cpp server_driver/Nats.h)
target_link_libraries(${client} PRIVATE tomlplusplus::tomlplusplus)
target_link_libraries(${client} PRIVATE redis++::redis++_static)
target_link_libraries(${client} PRIVATE cnats::nats_static)


add_executable(${server} server.cpp tools.h server_driver/Base.cpp server_driver/Base.h server_driver/Redis.cpp server_driver/Redis.h server_driver/Factory.cpp server_driver/Factory.h server_driver/Nats.cpp server_driver/Nats.h ServerConfig.cpp ServerConfig.h)
target_link_libraries(${server} PRIVATE tomlplusplus::tomlplusplus)
target_link_libraries(${server} PRIVATE redis++::redis++_static)
target_link_libraries(${server} PRIVATE cnats::nats_static)

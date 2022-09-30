cmake_minimum_required(VERSION 3.24)
project(vcpkg_1)


set(CMAKE_CXX_STANDARD 20)

find_package(tomlplusplus CONFIG REQUIRED)
find_package(redis++ CONFIG REQUIRED)
find_package(cnats CONFIG REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I/opt/homebrew/Cellar/pcapplusplus/22.05/include/pcapplusplus -I$(MACOS_SDK_HOME)/usr/include/netinet -L/opt/homebrew/Cellar/pcapplusplus/22.05/lib -lPcap++ -lPacket++ -lCommon++ -lpcap -lpthread -framework SystemConfiguration -framework CoreFoundation")
include_directories("/usr/local/include/pcapplusplus")
#-L/opt/homebrew/Cellar/pcapplusplus/22.05/lib -lPcap++ -lPacket++ -lCommon++ -lpcap -lpthread -framework SystemConfiguration -framework CoreFoundation
#find_package(PcapPlusPlus REQUIRED)
#include_directories(${PCAPPLUSPLUS_INCLUDE_DIRS})
add_executable(${PROJECT_NAME} main.cpp Config.cpp Config.h tools.h server_driver/Base.cpp server_driver/Base.h server_driver/Redis.cpp server_driver/Redis.h server_driver/Factory.cpp server_driver/Factory.h server_driver/Nats.cpp server_driver/Nats.h)
target_link_libraries(${PROJECT_NAME} PRIVATE tomlplusplus::tomlplusplus)
target_link_libraries(${PROJECT_NAME} PRIVATE redis++::redis++_static)
target_link_libraries(${PROJECT_NAME} PRIVATE cnats::nats_static)

#target_link_libraries(${PROJECT_NAME} ${PcapPlusPlus_LIBS})
#target_link_libraries(${PROJECT_NAME} libCommon++.a libPacket++.a libPcap++.a)
#target_link_libraries(vcpkg_1 PRIVATE})


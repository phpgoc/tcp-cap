cmake_minimum_required(VERSION 3.24)
project(vcpkg_1)


set(CMAKE_CXX_STANDARD 20)

find_package(tomlplusplus CONFIG REQUIRED)
include_directories("/usr/local/include/pcapplusplus")
#find_package(PcapPlusPlus REQUIRED)
#include_directories(${PCAPPLUSPLUS_INCLUDE_DIRS})
add_executable(${PROJECT_NAME} main.cpp Config.cpp Config.h tools.h)
target_link_libraries(${PROJECT_NAME} PRIVATE tomlplusplus::tomlplusplus)
#target_link_libraries(vcpkg_1 PRIVATE ${PCAPPLUSPLUS_LIBRARIES})


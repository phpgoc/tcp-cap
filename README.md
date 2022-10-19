# 编译

目前无法在任何非mac环境编译，即使是在mac也需要修改 set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  成 pkg-config --cflags --libs PcapPlusPlus
vcpkg 没有整合好 PcapPlusPlus

# 功能
 
- client 放在数据库所在的机器上，用于从数据库中读取数据，然后发送到server
- server 放在集中的服务器上，用于接收client发送过来的数据，然后写入到后端[数据库,或其他]

# 使用

- client 使用
```bash
#方法1
$ cp config.example.toml <client path>/config.toml
# modify config.toml
$ cd <client path> && ./client
#方法2
$ cd <client path> && ./client <config.json path>
```

- server 使用
```bash
#方法1
$ cp server_config.example.toml <server path>/server_config.toml
# modify server_config.toml
$ cd <server path> && ./server
#方法2
$ cd <server path> && ./server <server_config.toml path>
```

- 简单实用
```bash

make debug #第一次会需要很长时间，而且有pcapplusplus的bug，需要自行修改一下 
make config
cd Debug
# modify config.toml
# modify server_config.toml
./client
./server
```

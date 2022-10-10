# 编译


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


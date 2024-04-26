# chatserver
使用C++实现，基于muduo 库，nginx  tcp 负载均衡部署，服务器间通信使用redis 的发布-订阅功能的集群聊天服务器和客户端源码，


## 编译方式
cd build 
rm -rf *
cmake ..
make 
也可直接运行 compile.sh

## 项目运行需要的的环境
* C++11
* muduo库
* mysql
* nginx tcp 负载均衡 （nginx tcp 模式需要修改配置文件）
* redis
* json 库（已包含在thirdparty 文件夹）

## 项目介绍
* 使用muduo网络库作为项目的网络核心模块，提供高并发网络I/O服务，解耦网络和业务模块代码
* 使用json序列化和反序列化消息
* 配置nginx基于tcp的负载均衡，实现聊天服务器的集群功能，提高后端服务的并发能力
* 基于redis的发布-订阅功能，实现跨服务器的消息通信
* 使用mysql关系型数据库作为项目数据的落地存储
* 使用连接池提高数据库的数据存取性能













# chatserver
使用C++实现，基于muduo 库，nginx  tcp 负载均衡部署，服务器间通信使用redis 的发布-订阅功能的集群聊天服务器和客户端源码，


# 编译方式
cd build 
rm -rf *
cmake ..
make 
也可直接运行 compile.sh

# 项目运行需要的的环境
C++11
muduo库
mysql
nginx tcp 负载均衡 （nginx tcp 模式需要修改配置文件）
redis
json 库（已包含在thirdparty 文件夹）

# 项目结构介绍
项目代码放在 include 和 src 文件夹中













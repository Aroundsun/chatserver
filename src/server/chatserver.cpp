#include "chatserver.hpp"
#include"chatservice.hpp"

#include<json.hpp>


#include <functional>
#include<string>

using namespace std::placeholders;
using json = nlohmann::json;


ChatServer::ChatServer(EventLoop *loop,
           const InetAddress &listenAddr,
           const string &nameArg)
    : _server(loop, listenAddr, nameArg), _loop(loop)
{

    // 注册连接回调
    _server.setConnectionCallback(std::bind(&ChatServer::onConnection,this,_1));
    // 注册消息回调
    _server.setMessageCallback(std::bind(&ChatServer::onMessage,this,_1,_2,_3));
    //设置线程数量
    _server.setThreadNum(4);

}

void ChatServer::start()
{
    _server.start();
}

//上报连接相关的信息的回调函数
void ChatServer::onConnection(const TcpConnectionPtr& conn )
{
    if(!conn->connected())
    {
        //处理客户端异常断开
        chatservice::instance()->clientCloseExecption(conn);
        conn->shutdown();
    }
}    
    

//上报读写事件的相关信息的回调函数
void ChatServer::onMessage(const TcpConnectionPtr& conn,Buffer* buffer,Timestamp time)
{
    string buf = buffer->retrieveAllAsString();
    //数据的反序列化
    json js = json::parse(buf);   
    //解耦网络模块与业务模块
    chatservice::instance()->getMsgHandler(js["msgid"].get<int>())(conn,js,time);
    
}
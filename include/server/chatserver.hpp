#ifndef CHATSERVER_H_
#define CHATSERVER_H_

#include<muduo/net/TcpServer.h>
#include<muduo/net/EventLoop.h>
using namespace muduo;
using namespace muduo::net;

//聊天服务器主类
class ChatServer
{
public:
    //初始化聊天服务器对象
    ChatServer(EventLoop *loop,
            const InetAddress& listenAddr,
            const string& nameArg);
    //启动服务
    void start();

private:
    //上报连接相关的信息的回调函数
    void onConnection(const TcpConnectionPtr& );

    //上报读写事件的相关信息的回调函数
    void onMessage(const TcpConnectionPtr&,Buffer*,Timestamp);

    TcpServer _server;//组合muduo 库，实现服务器功能的类对象
    EventLoop *_loop; //指向事件循环的对象的指针
};


#endif  // CHATSERVER_H_
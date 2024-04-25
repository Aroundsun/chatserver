#ifndef CHATERVICE_H_
#define CHATERVICE_H_

#include<json.hpp>

#include<muduo/net/TcpConnection.h>
#include<mutex>
#include<unordered_map>
#include<functional>
#include"usermodel.hpp"
#include "offlinemessagemodel.hpp"
#include "friendmodel.hpp"
#include "groupmodel.hpp"
#include "redis.hpp"
using namespace muduo;
using namespace muduo::net;
using namespace std::placeholders;
using json = nlohmann::json;

//处理消息事件回调方法类型
using MsgHandler = std::function<void(const TcpConnectionPtr& conn,json js,Timestamp time)>;

//业务模块 --单例模式
class chatservice
{
public:
    //获取单例对象的接口函数
    static chatservice* instance();
    //处理登录业务
    void login(const TcpConnectionPtr& conn,json js,Timestamp time);
    //处理注册业务
    void reg(const TcpConnectionPtr& conn,json js,Timestamp time);
    //一对一聊天业务
    void oneChat(const TcpConnectionPtr& conn,json js,Timestamp time);
    //添加好友业务
    void addFriend(const TcpConnectionPtr& conn,json js,Timestamp time);
    //创建群聊业务
    void creatGroup(const TcpConnectionPtr& conn,json js,Timestamp time);
    //加入群聊业务
    void addGroup(const TcpConnectionPtr& conn,json js,Timestamp time);
    //群组聊天业务
    void groupChat(const TcpConnectionPtr& conn,json js,Timestamp time);
    // 处理注销业务
    void loginout(const TcpConnectionPtr &conn, json js, Timestamp time);
    //服务器异常断开，业务重置方法
    void reset();
    //处理客户端异常断开
    void clientCloseExecption(const TcpConnectionPtr & conn);
    //获取消息处理器
    MsgHandler getMsgHandler(int msgid);
    // 从redis消息队列中获取订阅的消息
    void handleRedisSubscribeMessage(int, string);
private:
    chatservice();

    //存储消息类型表
    std::unordered_map<int,MsgHandler> _msgHandlerMap;


    //存储在线用户的连接
    std::unordered_map<int,TcpConnectionPtr> _userConnMap;

    //定义互斥锁，保证 _userConnMap 的线程安全
    std::mutex _connMutex;

    //用户数据操作对象
    UserModel _userModel;
    //用户离线消息数据操作对象
    offlinemessagemodel _offMsgModel;
    //好友数据操作对象
    friendmodel _friendMode;
    //群聊数据操作对象
    groupmodel _groupmodel;

    //Redis 对象
    Redis _redis;
};
#endif 
#include "chatservice.hpp"

#include "public.hpp"

#include <iostream>
#include <list>
#include <muduo/base/Logging.h>
using namespace muduo;

#include "usermodel.hpp"
#include "groupmodel.hpp"
// 获取单例对象的接口函数
chatservice *chatservice::instance()
{
    static chatservice service;
    return &service;
}

chatservice::chatservice()
{
    _msgHandlerMap.insert({LOGIN_MSG, std::bind(&chatservice::login, this, _1, _2, _3)});
    _msgHandlerMap.insert({LOGIN_OUT_MSG, std::bind(&chatservice::loginout, this, _1, _2, _3)});
    _msgHandlerMap.insert({REG_MSG, std::bind(&chatservice::reg, this, _1, _2, _3)});
    _msgHandlerMap.insert({ONE_CHAT_MSG, std::bind(&chatservice::oneChat, this, _1, _2, _3)});
    _msgHandlerMap.insert({ADD_MSG, std::bind(&chatservice::addFriend, this, _1, _2, _3)});

    _msgHandlerMap.insert({CREAT_GROUP_MSG, std::bind(&chatservice::creatGroup, this, _1, _2, _3)});
    _msgHandlerMap.insert({ADD_GROUP_MSG, std::bind(&chatservice::addFriend, this, _1, _2, _3)});
    _msgHandlerMap.insert({GROUP_CHAT_MSG, std::bind(&chatservice::groupChat, this, _1, _2, _3)});

    // 连接redis服务器
    if (_redis.connect())
    {
        // 设置上报消息的回调
        _redis.init_notify_handler(std::bind(&chatservice::handleRedisSubscribeMessage, this, _1, _2));
    }
}

// 处理登录业务
void chatservice::login(const TcpConnectionPtr &conn, json js, Timestamp time)
{
    int id = js["id"];
    string pwd = js["password"];

    User user = _userModel.query(id);

    if (user.getId() != -1 && user.getPwd() == pwd)
    {
        if (user.getState() == "online")
        {
            // 用户已经登录
            json response;
            response["msgid"] = LOGIN_MSG_ACK;
            response["errno"] = 2;
            response["errmsg"] = "用户已经登录";
            conn->send(response.dump());
        }
        else
        {
            // 登录成功 记录用户的连接状态
            {
                lock_guard<mutex> lock(_connMutex);
                _userConnMap.insert({user.getId(), conn});
            }

            //用户id登陆成功后 向redis 订阅 channel(id)
            _redis.subscribe(id);
            // 登陆成功更新用户登陆状态
            user.setState("online");
            _userModel.update(user);
            json response;
            response["msgid"] = LOGIN_MSG_ACK;
            response["errno"] = 0;
            response["id"] = user.getId();
            response["name"] = user.getName();
            // 加载离线消息
            list<string> offmsglist = _offMsgModel.query(id);
            if (!offmsglist.empty())
            {
                response["offmsg"] = offmsglist;
                // 加载完成在数据库中删除该离线消息
                _offMsgModel.remove(id);
            }

            // 查询好友列表并返回
            list<User> friendList = _friendMode.query(id);
            if (!friendList.empty())
            {
                list<string> friendList2;
                for (User &user : friendList)
                {
                    json js;
                    js["id"] = user.getId();
                    js["name"] = user.getName();
                    js["state"] = user.getState();
                    friendList2.push_back(js.dump());
                }
                response["friends"] = friendList2;
            }
            conn->send(response.dump());
        }
    }
    else
    {
        // 登录失败
        json response;
        response["msgid"] = LOGIN_MSG_ACK;
        response["errno"] = 1;
        response["errmsg"] = "用户名或者密码错误";
        conn->send(response.dump());
    }
}
// 处理注册业务
void chatservice::reg(const TcpConnectionPtr &conn, json js, Timestamp time)
{
    string name = js["name"];
    string pwd = js["password"];

    User user;
    user.setName(name);
    user.setPwd(pwd);
    bool state = _userModel.insert(user);
    if (state)
    {
        // 注册成功
        json response;
        response["msgid"] = REG_MSG_ACK;
        response["errno"] = 0;
        response["id"] = user.getId();
        conn->send(response.dump());
    }
    else
    {
        // 注册失败
        json response;
        response["msgid"] = REG_MSG_ACK;
        response["errno"] = 1;
        response["ermsg"] = 1;
        response["id"] = user.getId();
        conn->send(response.dump());
    }
}

// 一对一聊天业务
/*
    msgid:
    to:
    from:
    fromname:
    msg:
*/
void chatservice::oneChat(const TcpConnectionPtr &conn, json js, Timestamp time)
{
    int toid = js["to"].get<int>();

    {
        lock_guard<mutex> lock(_connMutex);
        auto it = _userConnMap.find(toid);
        if (it != _userConnMap.end())
        {
            // toid 在线 转发消息
            conn->send(js.dump());
            return;
        }
    }
    //查询toid 是否在其他服务器在线

    User toUser = _userModel.query(toid).getId();
    if (toUser.getState() == "online")
    {
        _redis.publish(toid,js.dump());
        return;
    }
    else
    {
        // toid 不在线，存储离线消息
        _offMsgModel.insert(toid, js.dump());
    }
}

// 添加好友业务
/*
    msgid:
    userid:
    friendid:
*/
void chatservice::addFriend(const TcpConnectionPtr &conn, json js, Timestamp time)
{
    int userid = js["userid"].get<int>();
    int friendid = js["friendid"].get<int>();

    // 储存好友信息
    _friendMode.insert(userid, friendid);
}

// 创建群聊业务
/*
    msgid:
    id:  --创建人id
    groupname:
    groupdesc:
*/
void chatservice::creatGroup(const TcpConnectionPtr &conn, json js, Timestamp time)
{
    int id = js["id"].get<int>();
    string groupname = js["groupname"];
    string groupdesc = js["groupdesc"];
    Group group(-1, groupname, groupdesc);
    if (_groupmodel.creatGroup(group))
    {
        // 存储创建人信息
        _groupmodel.addGroup(id, group.getId(), "creator");
    };
}
// 加入群聊业务
/*
    msgid:
    id:  --添加者id
    groupid:
    groupname:
*/
void chatservice::addGroup(const TcpConnectionPtr &conn, json js, Timestamp time)
{
    int userid = js["id"].get<int>();
    int groupid = js["groupid"].get<int>();
    string groupname = js["groupname"];
    _groupmodel.addGroup(userid, groupid);
}
// 群组聊天业务
/*
    msgid:
    id:
    groupid:
    msg:
*/
void chatservice::groupChat(const TcpConnectionPtr &conn, json js, Timestamp time)
{
    int userid = js["id"].get<int>();
    int groupid = js["groupid"].get<int>();
    vector<int> useridVec = _groupmodel.queryGroupUsers(userid, groupid);

    lock_guard<mutex> lock(_connMutex);
    for (int id : useridVec)
    {
        //在当前服务器的连接表中查找id
        auto it = _userConnMap.find(id);
        if (it != _userConnMap.end())
        {
            // 转发群消息
            it->second->send(js.dump());
            return;
        }
        else
        {
            // 查询toid是否登录在其他服务其上
            User user = _userModel.query(id);
            if (user.getState() == "online")
            {
                //向redis 消息队列中订阅 toid的管道
                _redis.publish(user.getId(),js.dump());
                return;
            }
            else
            {
                // 存储离线群消息
                _offMsgModel.insert(id, js.dump());
                return;
            }
        }
    }
}
// 处理注销业务
void chatservice::loginout(const TcpConnectionPtr &conn, json js, Timestamp time)
{
    int userid = js["id"].get<int>();

    {
        lock_guard<mutex> lock(_connMutex);
        auto it = _userConnMap.find(userid);
        if (it != _userConnMap.end())
        {
            _userConnMap.erase(it);
        }
    }

    // 用户注销，相当于就是下线，在redis中取消订阅通道
    _redis.unsubscribe(userid); 

    // 更新用户的状态信息
    User user(userid, "", "", "offline");
    _userModel.update(user);
}
// 服务器异常断开，业务重置方法
void chatservice::reset()
{
    // 把状态为 online 的用户，设置成 "offline"
    _userModel.resetState();
}

// 处理客户端异常断开
void chatservice::clientCloseExecption(const TcpConnectionPtr &conn)
{
    int id;
    // 在connMap 表中删除该连接信息
    {
        lock_guard<mutex> lock(_connMutex);

        for (auto it = _userConnMap.begin(); it != _userConnMap.end(); ++it)
        {
            if (conn == it->second)
            {
                id = it->first;
                _userConnMap.erase(id);
                break;
            }
        }
    }


    // 用户注销，相当于就是下线，在redis中取消订阅通道
    _redis.unsubscribe(id); 


    if (id != -1) // id 有效
    {
        User user = _userModel.query(id);
        user.setState("offline");
        _userModel.update(user);
    }
}

// 获取消息处理器
MsgHandler chatservice::getMsgHandler(int msgid)
{
    auto it = _msgHandlerMap.find(msgid);
    if (it == _msgHandlerMap.end())
    {
        return [=](const TcpConnectionPtr &conn, json js, Timestamp time)
        {
            LOG_ERROR << "msgid" << msgid << "can not find Handler !!!";
        };
    }
    return _msgHandlerMap[msgid];
}
// 从redis消息队列中获取订阅的消息
void chatservice::handleRedisSubscribeMessage(int userid, string msg)
{
    lock_guard<mutex> lock(_connMutex);
    auto it = _userConnMap.find(userid);
    if (it != _userConnMap.end())
    {
        it->second->send(msg);
        return;
    }
    // 存储该用户的离线消息
    _offMsgModel.insert(userid, msg);
}

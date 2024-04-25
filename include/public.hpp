#ifndef PUBLIC_H_
#define PUBLIC_H_

enum EnMsgType
{
    LOGIN_MSG = 1, // 登录消息
    LOGIN_MSG_ACK, // 登录消息确认码
    LOGIN_OUT_MSG,  // 注销消息
    REG_MSG,       // 注册消息
    REG_MSG_ACK,   // 注册信息确认码
    
    ONE_CHAT_MSG,  // 聊天消息
    ADD_MSG,       // 添加好友

    CREAT_GROUP_MSG, // 创建群组
    ADD_GROUP_MSG,   // 加入群组
    GROUP_CHAT_MSG,  // 群聊消息

};

#endif // PUBLIC_H_
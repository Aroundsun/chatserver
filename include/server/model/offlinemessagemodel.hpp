#ifndef OFFLINEMESSAGEMODEL_H_
#define OFFLINEMESSAGEMODEL_H_
#include<string>
#include<list>
using namespace std;

class offlinemessagemodel
{
public:

    //存储用户离线消息
    bool insert(int userid,string offmsg);
    //删除用户离线消息
    bool remove(int userid);
    //查询用户离线消息
    list<string> query(int userid);
};
#endif
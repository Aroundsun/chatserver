#ifndef GROUPMODEL_H_
#define GROUPMODEL_H_

#include"Group.hpp"
#include"groupuser.hpp"
#include"db.hpp"
class groupmodel
{
public:
    //创建群聊
    bool creatGroup(Group &group);
    //加入群聊
    bool addGroup(int userid,int groupid,string grouprole = "normal");
    //查询用户所在的群聊信息
    vector<Group> queryGroups(int userid);
    //根据指定的groupid 查询群组用户id 列表
    vector<int> queryGroupUsers(int userid,int groupid);



};

#endif
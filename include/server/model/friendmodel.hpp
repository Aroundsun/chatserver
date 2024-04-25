#ifndef FRIENDMODEL_H_
#define FRIENDMODEL_H_

#include<list>
#include<usermodel.hpp>
class friendmodel
{
public:
    //添加好友
    bool insert(int id1,int id2);
    //返回用户的好友列表
    list<User> query(int id);


    
};
#endif
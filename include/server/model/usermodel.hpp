#ifndef USERMODEL_H_
#define USERMODEL_H_
#include"User.hpp"
//User 表的数据操作类
class UserModel
{
public:
    //插入数据
    bool insert(User & user);
    //根据id 查询数据
    User query(int id);
    //更新数据
    bool update(User& user);

    //重置所有用户的状态信息
    void resetState();



};


#endif
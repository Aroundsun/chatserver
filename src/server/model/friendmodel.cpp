#include "friendmodel.hpp"
#include"db.hpp"
#include<usermodel.hpp>
// 添加好友
bool friendmodel::insert(int id1, int id2)
{
    // 组装SQL语句
    char sql[1024] = {};
    sprintf(sql, "insert into friend(userid,friendid) values(%d,%d)",id1,id2);
    MySQL mysql;
    if (mysql.connect())
    {
        if (mysql.update(sql))
        {
            return true;
        }
    }
    return false;
}
// 返回用户的好友列表
list<User> friendmodel::query(int id)
{
    // 组装SQL
    char sql[1024] = {};
    sprintf(sql, "select a.id,a.name,a.state from user inner join friend b.friendid where a.id = %d ", id);
    list<User> friendList;
    MySQL mysql;
    if (mysql.connect())
    {
        MYSQL_RES *res = mysql.query(sql);
        if (res != nullptr)
        {
            //把userid 的所有离线消息 放入offmsglist 中
            MYSQL_ROW row;

            while((row = mysql_fetch_row(res)) != nullptr)
            {
                User user;
                user.setId(atoi(row[0]));
                user.setName(row[1]);
                user.setState(row[2]);
                friendList.push_back(user);
            }
            mysql_free_result(res);
        }
    }
    return friendList;
}
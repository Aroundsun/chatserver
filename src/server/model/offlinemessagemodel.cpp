#include "offlinemessagemodel.hpp"
#include "db.hpp"
// 存储用户离线消息
bool offlinemessagemodel::insert(int userid, string offmsg)
{
    // 组装SQL
    char sql[1024] = {};
    sprintf(sql, "insert into  offlinemessage(userid,message) values(%d,%s)", userid, offmsg.c_str());
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
// 删除用户离线消息
bool offlinemessagemodel::remove(int userid)
{ 
    // 组装SQL语句
    char sql[1024] = {};
    sprintf(sql, "delete from offlinemessage,where userid = %d",userid);
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
// 查询用户离线消息
list<string> offlinemessagemodel::query(int userid)
{
    // 组装SQL
    char sql[1024] = {};
    sprintf(sql, "select message from offlinemessage where userid = %d ", userid);
    list<string> offmsglist;
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
                offmsglist.push_back(row[0]);
            }
            mysql_free_result(res);
        }
    }
    return offmsglist;
}
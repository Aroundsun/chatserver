#include "groupmodel.hpp"

// 创建群聊
bool groupmodel::creatGroup(Group& group)
{
    // 组装SQL语句
    char sql[1024] = {};
    sprintf(sql, "insert into allgroup(id,groupname,groupdesc) values(%d,'%s','%s')",
            group.getId(), group.getName().c_str(), group.getDesc().c_str());
    MySQL mysql;
    if (mysql.connect())
    {
        if (mysql.update(sql))
        {
            // 获取插入成功的群组数据的主键id
            group.setId(mysql_insert_id(mysql.getConnection()));
            // group.set
            return true;
        }
    }
    return false;
}
// 加入群聊
bool groupmodel::addGroup(int userid, int groupid, string grouprole)
{
    // 组装SQL语句
    char sql[1024] = {};
    sprintf(sql, "insert into groupuser(groupid,userid,grouprole) values(%d,%d,'%s')",
            userid, groupid, grouprole.c_str());
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
// 查询用户所在的群聊信息
vector<Group> groupmodel::queryGroups(int userid)
{

    /*
        根据userid 在groupuser 表查询出用户所属的群组信息
    */
    char sql[1024] = {};
    sprintf(sql, "select a.id,a.groupname,a.groupdesc from allgroup a inner \
        join groupuser b on a.id = b.groupid where b.userid = %d",
            userid);
    MySQL mysql;
    vector<Group> GroupVec;
    if (mysql.connect())
    {
        MYSQL_RES *res = mysql.query(sql);
        if (res != nullptr)
        {
            MYSQL_ROW row;
            while ((row = mysql_fetch_row(res)) != nullptr)
            {
                Group group;
                group.setId(atoi(row[0]));
                group.setName(row[1]);
                group.setDesc(row[2]);
                GroupVec.push_back(group);
            }
            mysql_free_result(res);
        }
    }
    // 查询该用户所属的所有群组的所有成员信息
    for (Group &group : GroupVec)
    {
        sprintf(sql, "select a.id,a.name,a.state,b.grouprole from user a \
            inner join groupuser b on b.userid = a.id where b.groupid=%d",
                group.getId());
        if (mysql.connect())
        {
            MYSQL_RES *res = mysql.query(sql);
            if (res != nullptr)
            {
                MYSQL_ROW row;
                while ((row = mysql_fetch_row(res)) != nullptr)
                {
                    GroupUser user;
                    user.setId(atoi(row[0]));
                    user.setName(row[1]);
                    user.setState(row[2]);
                    user.setRole(row[3]);
                    group.getGroupUsers().push_back(user);
                }
                mysql_free_result(res);
            }
        }
    }
    return GroupVec;
}
// 根据指定的groupid 查询群组用户id 列表
vector<int> groupmodel::queryGroupUsers(int userid, int groupid)
{
    char sql[1024] = {0};
    sprintf(sql, "select userid from groupuser where groupid = %d and userid != %d", groupid, userid);

    vector<int> idVec;
    MySQL mysql;
    if (mysql.connect())
    {
        MYSQL_RES *res = mysql.query(sql);
        if (res != nullptr)
        {
            MYSQL_ROW row;
            while ((row = mysql_fetch_row(res)) != nullptr)
            {
                idVec.push_back(atoi(row[0]));
            }
            mysql_free_result(res);
        }
    }
}
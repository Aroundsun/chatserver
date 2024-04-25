#ifndef GROUP_H_
#define GROUP_H_

#include <string>
#include <vector>
using namespace std;
#include "groupuser.hpp"

// Gruop表的ORM类
class Group
{
public:
    Group(int id = -1, string name = "", string desc = "")
    {
        this->id = id;
        this->name = name;
        this->desc = desc;
    }
    void setId(int id) { this->id = id; }
    void setName(string name) { this->name = name; }
    void setDesc(string desc) { this->desc = desc; }

    int getId() { return this->id; }
    string getName() { return this->name; }
    string getDesc() { return this->desc = desc; }
    vector<GroupUser>& getGroupUsers() { return this->users; }

protected:
    int id;             // 组号
    string name;        // 组名
    string desc;        // 群组的描述
    vector<GroupUser> users; // 群组的成员列表
};

#endif
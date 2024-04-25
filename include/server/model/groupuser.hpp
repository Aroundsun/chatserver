#ifndef GROUPUSER_H_
#define GROUPUSER_H_
#include "Group.hpp"
#include "User.hpp"
#include <string>
using namespace std;
class GroupUser : public User
{
public:
    // 构造函数
    GroupUser(string role = "normal") : User()
    {
        this->role = role;
    }
    void setRole(string role)
    {
        this->role = role;
    }
    string getRole()
    {
        return role;
    }

private:
    string role;
};
#endif
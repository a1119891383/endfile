#ifndef MYSQLUTIL_H
#define MYSQLUTIL_H
#include <mysql/mysql.h>
#include <string>
#include<list>
#include"entity.h"
class MysqlUtil
{
private:
    MYSQL *conn;
    MYSQL_RES *result;
    MYSQL_ROW row;
public:
    int initUtil();
    MysqlUtil();
    ~MysqlUtil();
    bool loginSQL(int uid,std::string pwd);
    bool insertUsers(User);
    User selectUser(int uid);//ok
    bool chageUser(User);

    bool insertGroup(Group);
    bool changeGroup(Group);
    Group selectGroup(int gid);//ok

    bool insertGroupShip(GroupShip);
    bool changeGroupShip(GroupShip);
    std::list<GroupShip> selectGroupShip(int gid,int status);//ok
    bool deleteGroupShip(GroupShip ship);

    bool insertFriend(Friends );
    bool changeFriends(int id,int status);
    std::list<Friends> selectFriends(int uid,int status);//ok
    bool deleteFriends(int id);

    bool insertMessage(Message);
    std::list<Message> selectMessage(int recv_id);//ok
    bool deleteMessage(int id);

};
#endif
#include"MysqlUtil.h"
#include<stdio.h>
#include<iostream>
#include<list>
#include<sstream>
int MysqlUtil::initUtil()
{
    this->conn = mysql_init(NULL);
    if(!mysql_real_connect(this->conn,"127.0.0.1","root","zhgsb1314","graduate",3306,0,0))
    {
        printf("connect fail\n");
    }
    printf("connect success\n");
    return 1;

}
MysqlUtil::MysqlUtil()
{
    initUtil();
}

bool MysqlUtil::loginSQL(int uid,std::string pwd)
{
    std::string sql;
    char buff[1024];
    sprintf(buff,"select id,pwd from `users` where id = %d and pwd = \"%s\"",uid,pwd.data());
    sql = buff;
    int ret = mysql_query(conn,sql.data());
    if(ret)
    {
        printf("Query failed (%s)\n", mysql_error(conn));
        return false;
    }
    //查询数据行数
    if(mysql_affected_rows(conn))
    {
        return true;
    }
    std::cout << sql <<std::endl;
    return false;
}
//测试成功
bool MysqlUtil::MysqlUtil::insertUsers(User user)
{
    std::string sql;
    char buff[1024];
    sprintf(buff,"insert into `users`(id,pwd,uname,content,tel) values(%d,\"%s\",\"%s\",\"%s\",\"%s\")",user.uid,user.password.data(),user.name.data(),user.content.data(),user.tel.data());
    sql = buff;
    std::cout << sql <<std::endl;
    int ret = mysql_query(conn,sql.data());
    if(ret)
    {
        printf("Query failed (%s)\n", mysql_error(conn));
        return false;
    }
    if (!(result = mysql_store_result(conn))) //获得sql语句结束后返回的结果集 
    {
        printf("Couldn't get result from %s\n", mysql_error(conn));
        return false;
    }
    if(mysql_affected_rows(conn))return true;
    return false;
};
//测试成功
bool MysqlUtil::chageUser(User user)
{
    std::string sql;
    char buff[1024];
    sprintf(buff,"update `users` set uname = \"%s\",content = \"%s\",tel = \"%s\" where id = %d",user.name.data(),user.content.data(),user.tel.data(),user.uid);
    sql = buff;
    int ret = mysql_query(conn,sql.data());
    if(ret)
    {
        printf("Query failed (%s)\n", mysql_error(conn));
        return false;
    }
    if(mysql_affected_rows(conn))return true;
    return false;
};
//测试成功
User MysqlUtil::selectUser(int uid)
{
    std::cout << "start selectUser" <<std::endl;
    User user;
    std::string sql;
    char buff[1024];
    sprintf(buff,"select id,pwd,uname,content,tel from `users` where id = %d",uid);
    sql = buff;
    int ret = mysql_query(conn,sql.data());
    if(ret)
    {
        printf("Query failed (%s)\n", mysql_error(conn));
        return user;
    }
    //查询数据行数
    if(mysql_affected_rows(conn)== 0)return user;

    result = mysql_store_result(conn);

    row = mysql_fetch_row(result);
    std::string temp(row[0]);
    user.uid = stoi(temp);
    user.password = row[1];
    user.name = row[2];
    user.content = row[3];
    user.tel = row[4];

    mysql_free_result(result);

    return user;
}
//ok
bool MysqlUtil::insertGroup(Group group)
{
    User user;
    std::string sql;
    char buff[1024];
    sprintf(buff,"insert into `group`(gid,gname,introduction) values(%d,\"%s\",\"%s\")",group.gid,group.name.data(),group.introduction.data());
    sql = buff;
    int ret = mysql_query(conn,sql.data());
    if(ret)
    {
        printf("Query failed (%s)\n", mysql_error(conn));
        return false;
    }
    if (!(result = mysql_store_result(conn))) //获得sql语句结束后返回的结果集 
    {
        printf("Couldn't get result from %s\n", mysql_error(conn));
        return false;
    }
    if(mysql_affected_rows(conn))return true;
    return false;
};
//ok
bool MysqlUtil::changeGroup(Group group)
{
    std::string sql;
    char buff[1024];
    sprintf(buff,"update `group` set gname = \"%s\",introduction = \"%s\" where gid = %d",group.name.data(),group.introduction.data(),group.gid);
    sql = buff;
    int ret = mysql_query(conn,sql.data());
    if(ret)
    {
        printf("Query failed (%s)\n", mysql_error(conn));
        return false;
    }
    if(mysql_affected_rows(conn))return true;
    return false;
};
//ok
Group MysqlUtil::selectGroup(int gid)
{
    Group group;
    std::string sql;
    char buff[1024];
    sprintf(buff,"select gid,gname,introduction from `group` where gid = %d",gid);
    sql = buff;
    int ret = mysql_query(conn,sql.data());
    if(ret)
    {
        printf("Query failed (%s)\n", mysql_error(conn));
        return group;
    }
    //查询数据行数
    if(mysql_affected_rows(conn)==0)return group;

    result = mysql_store_result(conn);

    row = mysql_fetch_row(result);
    std::string temp(row[0]);
    group.gid = stoi(temp);
    group.name = row[1];
    group.introduction = row[2];

    mysql_free_result(result);

    return group;

};
//ok
bool MysqlUtil::insertGroupShip(GroupShip groupShip)
{
    std::string sql;
    char buff[1024];
    sprintf(buff,"insert into groupShip(gid,uid,status) values(%d,%d,%d)",groupShip.gid,groupShip.uid,groupShip.status);
    sql = buff;
    int ret = mysql_query(conn,sql.data());
    if(ret)
    {
        printf("Query failed (%s)\n", mysql_error(conn));
        return false;
    }
    if (!(result = mysql_store_result(conn))) //获得sql语句结束后返回的结果集 
    {
        printf("Couldn't get result from %s\n", mysql_error(conn));
        return false;
    }
    if(mysql_affected_rows(conn))return true;
    return false;
};
//ok
bool MysqlUtil::changeGroupShip(GroupShip groupShip)
{
    std::string sql;
    char buff[1024];
    sprintf(buff,"update groupShip set status = %d where id = %d",groupShip.status,groupShip.id);
    sql = buff;
    int ret = mysql_query(conn,sql.data());
    if(ret)
    {
        printf("Query failed (%s)\n", mysql_error(conn));
        return false;
    }
    if(mysql_affected_rows(conn))return true;
    return false;
};
//ok status 0 申请 1 群主 2成员
std::list<GroupShip> MysqlUtil::selectGroupShip(int uid,int status)
{
    std::list<GroupShip> glist;
    GroupShip groupShip;
    std::string sql;
    char buff[1024];
    sprintf(buff,"select id,gid,uid,status from groupShip where uid = %d and status = %d",uid,status);
    sql = buff;
    int ret = mysql_query(conn,sql.data());
    if(ret)
    {
        printf("Query failed (%s)\n", mysql_error(conn));
        return glist;
    }
    //查询数据行数
    if(mysql_affected_rows(conn)==0)return glist;

    result = mysql_store_result(conn);

    std::string temp;
    while(row = mysql_fetch_row(result))
    {
        temp = row[0];
        groupShip.id = stoi(temp);
        temp = row[1];
        groupShip.gid = stoi(temp);
        temp = row[2];
        groupShip.uid = stoi(temp);
        temp = row[3];
        groupShip.status = stoi(temp);
        glist.push_back(groupShip);

    }

    mysql_free_result(result);

    return glist;
};
bool MysqlUtil::deleteGroupShip(GroupShip groupShip)
{
};


bool MysqlUtil::insertFriend(Friends f)
{
    std::string sql;
    char buff[1024];
    sprintf(buff,"insert into friends(frist_id,second_id,status) values(%d,%d,%d)",f.frist_id,f.second_id,f.status);
    sql = buff;
    int ret = mysql_query(conn,sql.data());
    if(ret)
    {
        printf("Query failed (%s)\n", mysql_error(conn));
        return false;
    }
    if (!(result = mysql_store_result(conn))) //获得sql语句结束后返回的结果集 
    {
        printf("Couldn't get result from %s\n", mysql_error(conn));
        return false;
    }
    if(mysql_affected_rows(conn))return true;
    return false;
};
//ok
bool MysqlUtil::changeFriends(int id,int status)
{
    std::string sql;
    char buff[1024];
    sprintf(buff,"update friends set status = %d where id = %d",status,id);
    sql = buff;
    int ret = mysql_query(conn,sql.data());
    if(ret)
    {
        printf("Query failed (%s)\n", mysql_error(conn));
        return false;
    }
    if(mysql_affected_rows(conn))return true;
    return false;
};
std::list<Friends> MysqlUtil::selectFriends(int uid,int status)
{
    //status = 1 好友申请
    //status = 2 好友
    std::list<Friends> flist;
    Friends f;
    std::string sql;
    char buff[1024];
    if(status == 2)sprintf(buff,"select id,frist_id,second_id,status from friends where (frist_id = %d or second_id = %d) and status = 2",uid,uid);
    else sprintf(buff,"select id,frist_id,second_id,status from friends where (frist_id = %d and status = 1) or (second_id = %d and status = 0)",uid,uid);
    sql = buff;
    int ret = mysql_query(conn,sql.data());
    if(ret)
    {
        printf("Query failed (%s)\n", mysql_error(conn));
        return flist;
    }
    //查询数据行数
    if(mysql_affected_rows(conn)==0)return flist;

    result = mysql_store_result(conn);

    std::string temp;
    while(row = mysql_fetch_row(result))
    {
        temp = row[0];
        f.id = stoi(temp);
        temp = row[1];
        f.frist_id = stoi(temp);
        temp = row[2];
        f.second_id = stoi(temp);
        temp = row[3];
        f.status = stoi(temp);
        flist.push_back(f);

    }

    mysql_free_result(result);

    return flist;
};
bool MysqlUtil::deleteFriends(int id)
{

};


//ok
bool MysqlUtil::insertMessage(Message message)
{
    std::string sql;
    char buff[1024];
    sprintf(buff,"insert into message(send_id,recv_id,send_time,msg_type,context) values(%d,%d,%d,%ld,\"%s\")",message.send_id,message.recv_id,message.time,message.msgType,message.context.data());
    sql = buff;
    std::cout << sql <<std::endl;
    int ret = mysql_query(conn,sql.data());
    if(ret)
    {
        printf("Query failed (%s)\n", mysql_error(conn));
        return false;
    }
    if (!(result = mysql_store_result(conn))) //获得sql语句结束后返回的结果集 
    {
        printf("Couldn't get result from %s\n", mysql_error(conn));
        return false;
    }
    if(mysql_affected_rows(conn))return true;
    return false;
}
//ok
std::list<Message> MysqlUtil::selectMessage(int recv_id)
{
    std::list<Message> mlist;
    Message message;
    std::string sql;
    char buff[1024];
    sprintf(buff,"select id,send_id,recv_id,send_time,msg_type,context from message where recv_id = %d",recv_id);
    sql = buff;
    std::cout << sql <<std::endl;
    int ret = mysql_query(conn,sql.data());
    if(ret)
    {
        printf("Query failed (%s)\n", mysql_error(conn));
        return mlist;
    }
    //查询数据行数
    //std::cout << mysql_affected_rows(conn) << std::endl;

    result = mysql_store_result(conn);

    std::string temp;
    while(row = mysql_fetch_row(result))
    {
        temp = row[0];
        message.id = stoi(temp);//id
        temp = row[1];
        message.send_id = stoi(temp);//sendid
        temp = row[2];
        message.recv_id = stoi(temp);//recvid
        temp = row[3];
        long times;
        std::stringstream eam;
        eam << temp;
        eam >> times;
        message.time = times;//sendtime
        temp = row[4];
        message.msgType= stoi(temp);
        message.context= row[5];
        mlist.push_back(message);

    }

    mysql_free_result(result);
    
    return mlist;
}
//ok
bool MysqlUtil::deleteMessage(int recv_id)
{
    std::string sql;
    char buff[1024];
    sprintf(buff,"delete from message where recv_id = %d",recv_id);
    sql = buff;
    std::cout << sql <<std::endl;
    int ret = mysql_query(conn,sql.data());
    if(ret)
    {
        printf("Query failed (%s)\n", mysql_error(conn));
        return false;
    }
    if (!(result = mysql_store_result(conn))) //获得sql语句结束后返回的结果集 
    {
        printf("Couldn't get result from %s\n", mysql_error(conn));
        return false;
    }
    if(mysql_affected_rows(conn))return true;
    return false;
}
#ifndef USER_H
#define USER_H
#include<string>
struct User
{
    int uid;
    std::string password;
    std::string name;
    std::string content;
    std::string tel;
};
struct Group
{
    int gid;
    std::string name;
    std::string introduction;
};
struct Friends
{
    int id;
    int frist_id;
    int second_id;
    int status; // 0(1->2申请)  1(2->1) 2通过
};
struct GroupShip
{
    int id;
    int gid;
    int uid;
    int status; //0申请 1群主 2群员 
};
struct Message
{
    int id;
    time_t time;
    int send_id;
    int recv_id;
    int msgType;  //文字 或者 图片名称
    std::string context;
};
struct Video
{
    int id;
    int send_id;
    int recv_id;
    std::string photo;
};
struct MyFile
{
    int id;
    int number;
    int size;
    std::string fileName;
    std::string content; //每次传送1024B


};

#endif
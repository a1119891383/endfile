#ifndef SERVICE_H
#define SERVICE_H

#include<string>
#include<map>
#include"MsgQueue.h"
#include"Request.h"
#include"MysqlUtil.h"
class LiveMap;
class Service
{
public:
    Service(MsgQueue<Request *>* re);
    ~Service();
    void run();
    void doService();//解析repuest
    bool _exit;
    MsgQueue<Request *>* request;
    typedef void (Service::*Fun_ptr)(std::string,EpollData *);
    std::map<std::string,Fun_ptr> methodsMap;
    MysqlUtil *mysql_;
    LiveMap *liveMap;
private:
    void login(std::string data,EpollData *);//登陆
    void Register(std::string data,EpollData *);
    void friendList(std::string data,EpollData *);
    void messageList(std::string data,EpollData *);
    void groupList(std::string data,EpollData *);
    void sendMessage(std::string data,EpollData *);
    void Information(std::string data,EpollData *);
    void groupMeberList(std::string data,EpollData *);
    void upload(std::string data,EpollData *);
    void download(std::string data,EpollData *);
    void addFriend(std::string data,EpollData *);
    void addGroup(std::string data,EpollData *);
    void createGroup(std::string data,EpollData *);
    void sendGroupMessage(std::string data,EpollData *);
    
    
};


#endif

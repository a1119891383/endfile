#include"Service.h"
#include"json/json.h"
#include"Epollor.h"
#include<iostream>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include"LiveMap.h"
Service::Service(MsgQueue<Request *>* re):request(re) 
{
    mysql_ = new MysqlUtil();
    methodsMap.insert(make_pair("login",&Service::login));
    methodsMap.insert(make_pair("Register",&Service::Register));
    methodsMap.insert(make_pair("friendList",&Service::friendList));
    methodsMap.insert(make_pair("messageList",&Service::messageList));
    methodsMap.insert(make_pair("groupList",&Service::groupList));
    methodsMap.insert(make_pair("sendMessage",&Service::sendMessage));
    methodsMap.insert(make_pair("Information",&Service::Information));
    methodsMap.insert(make_pair("groupMeberList",&Service::groupMeberList));
    methodsMap.insert(make_pair("addFriend",&Service::addFriend));
    methodsMap.insert(make_pair("addGroup",&Service::addGroup));
    methodsMap.insert(make_pair("createGroup",&Service::createGroup));
    methodsMap.insert(make_pair("sendGroupMessage",&Service::sendGroupMessage));
}
Service::~Service() {}
void Service::run()
{
    while(!_exit)
    {
        auto args = request->get_msg();
        std::cout <<"start request run" << std::endl;
        Json::Value value;
        Json::Reader reader;
        if (reader.parse(args->_data, value)){
            
            if(!value["method"].isNull())
            {
                std::string method = value["method"].asString();
                std::cout << "request method:" << method << std::endl;
                std::string data;
                if(!value["data"].isNull())
                {
                    if(method=="errorExit")
                    {
                        //std::cout << "移除在线列表"<<std::endl;
                        liveMap->deleteUser(args->_epollData);
                        continue;
                    }
                    
                    //std::cout << "数据解析"<<std::endl;
                    data = Json::FastWriter().write(value["data"]);
                }
                (this->*methodsMap[method])(data,args->_epollData);
            }
        }
    }
}
void Service::login(std::string data,EpollData *epollData)
{
    std::cout << "run login start" << std::endl;
    Json::Value value;
    Json::Reader reader;
    int uid;
    std::string pwd;
    if (reader.parse(data, value))
    {
        if(!value["uid"].isNull())
        {
            uid = value["uid"].asInt();
        }
        if(!value["pwd"].isNull())
        {
            pwd = value["pwd"].asString();
        }
    }
    //数据库查询
    auto user = mysql_->selectUser(uid);

    if(!user.password.compare(pwd))
    {
        value = Json::objectValue;
        value["method"] = "responseLogin";
        value["data"]["uid"] = uid;
        value["data"]["name"] = user.name;
        value["data"]["content"] = user.content;
        auto test = Json::FastWriter().write(value);
        std::cout << test <<std::endl;
        epollData->sendResponse(test);
        //注册在线列表
        //std::cout<< "注册在线列表"<<std::endl;
        liveMap->insertUser(uid,epollData);
    }
    else
    {
        value = Json::objectValue;
        value["method"] = "responseLogin";
        value["data"]["name"] = "error";
        epollData->sendResponse(Json::FastWriter().write(value));
    }
};

void Service::Register(std::string data,EpollData *edata)
{
    std::cout << "run Register start" << std::endl;
    Json::Value value;
    Json::Reader reader;
    User user;
    if (reader.parse(data, value))
    {
        user.uid = value["uid"].asInt();
        user.password = value["pwd"].asString();
        user.name = value["name"].asString();
        user.content = value["content"].asString();
        user.tel = value["tel"].asString();
    }
    if(mysql_->insertUsers(user))
    {
        value = Json::objectValue;
        value["method"] = "responseRegister";
        value["data"] = "success";
        edata->sendResponse(Json::FastWriter().write(value));
    }
    else
    {
        value = Json::objectValue;
        value["method"] = "responseRegister";
        value["data"]= "error";
        edata->sendResponse(Json::FastWriter().write(value));
    }
}

void Service::friendList(std::string data,EpollData *edata)
{
    Json::Value value;
    Json::Reader reader;
    if(reader.parse(data, value))
    {
        auto uid = value["uid"].asInt();
        auto listFriend = mysql_->selectFriends(uid,2);
        std::list<Friends>::iterator it1;
        int id = 0;
        value = Json::objectValue;
        value["method"] = "responseFriendList";
        value["data"] = Json::arrayValue;
        int num = 0;
        for(it1 = listFriend.begin();it1!=listFriend.end();it1++)
        {
            auto temp = *it1;
            if(temp.frist_id == uid)id = temp.second_id;
            else id = temp.frist_id;
            std::cout << "friend:" << id << std::endl;
            auto user = mysql_->selectUser(id);
            value["data"][num]["uid"] = user.uid;
            value["data"][num]["name"] = user.name;
            value["data"][num]["tel"] = user.tel;
            value["data"][num]["content"] = user.content;
            num++;
        }
        std::string out = Json::FastWriter().write(value);
        std::cout << "friendList:" << out << std::endl;
        edata->sendResponse(out);
        
    }
}
void Service::groupList(std::string data,EpollData *edata)
{
    Json::Value value;
    Json::Reader reader;
    if(reader.parse(data, value))
    {
        auto uid = value["uid"].asInt();
        auto list1 = mysql_->selectGroupShip(uid,1);
        auto list2 = mysql_->selectGroupShip(uid,2);
        std::list<GroupShip>::iterator it1,it2;
        value = Json::objectValue;
        value["method"] = "responseGroupList";
        value["data"] = Json::arrayValue;
        int num = 0;
        for(it1 = list1.begin();it1!=list1.end();it1++)
        {
            auto group = mysql_->selectGroup((*it1).gid);
            value["data"][num]["gid"] = group.gid;
            value["data"][num]["name"] = group.name;
            value["data"][num]["introduction"] = group.introduction;
            num++;
        }
        for(it2 = list2.begin(); it2!=list2.end();it2++)
        {
            auto group = mysql_->selectGroup((*it2).gid);
            value["data"][num]["gid"] = group.gid;
            value["data"][num]["name"] = group.name;
            value["data"][num]["introduction"] = group.introduction;
            num++;
        }
        edata->sendResponse(Json::FastWriter().write(value));
    }
}

void Service::messageList(std::string data,EpollData *edata)
{
    Json::Value value;
    Json::Reader reader;
    if(reader.parse(data, value))
    {
            auto uid = value["uid"].asInt();
            auto ret = mysql_->selectMessage(uid);
            mysql_->deleteMessage(uid);
            std::list<Message>::iterator it1;
            value = Json::objectValue;
            value["method"] = "responseMessageList";
            value["data"] = Json::arrayValue;
            int num = 0;
            for(it1 = ret.begin();it1!=ret.end();it1++)
            {
                value["data"][num]["id"] = (*it1).id;
                value["data"][num]["send_id"] = (*it1).send_id;
                value["data"][num]["recv_id"] = (*it1).recv_id;
                value["data"][num]["msgType"] = (*it1).msgType;
                value["data"][num]["context"] = (*it1).context;
                value["data"][num]["time"] = (*it1).time;
                num++;
            }
            edata->sendResponse(Json::FastWriter().write(value));
        
    }
}
void Service::sendGroupMessage(std::string data,EpollData *edata)
{

}
void Service::sendMessage(std::string data,EpollData *edata)
{
    Json::Value value;
    Json::Reader reader;
    Message message;
    if(reader.parse(data, value))
    {
        //解析数据
        if(!value["send_id"].isNull())
        {
            message.send_id = value["send_id"].asInt();
            message.recv_id = value["recv_id"].asInt();
            message.msgType = value["msgType"].asInt();
            message.context = value["context"].asString();
            message.time = value["time"].asLargestInt();
        }
        // 是否在线
        auto ret = liveMap->findUser(message.recv_id);
        if(ret == NULL) mysql_->insertMessage(message);
        else
        {
            value = Json::objectValue;
            value["method"] = "SinMessage";
            value["data"] = Json::objectValue;
            value["data"]["send_id"] = message.send_id;
            value["data"]["recv_id"] = message.recv_id;
            value["data"]["msgType"] = message.msgType;
            value["data"]["context"] = message.context;
            value["data"]["time"] = message.time;
            ret->sendResponse(Json::FastWriter().write(value));
        }
    }
}

void Service::Information(std::string data,EpollData *edata)
{
    Json::Value value;
    Json::Reader reader;
    if(reader.parse(data, value))
    {
        // 0 个人信息 1群聊信息
        int ty = value["type"].asInt();
        int id = value["id"].asInt();
        value = Json::objectValue;
        
        if(ty == 0)
        {
            value["method"] = "responseInformationUser";
            auto user = mysql_->selectUser(id);
            value["data"] = Json::objectValue;
            value["data"]["uid"] = user.uid;
            value["data"]["name"] = user.name;
            value["data"]["content"] = user.content;
            value["data"]["tel"] = user.tel;
            edata->sendResponse(Json::FastWriter().write(value));

        }
        else
        {
            value["method"] = "responseInformationGroup";
            auto group = mysql_->selectGroup(id);
            value["data"] = Json::objectValue;
            value["data"]["uid"] = group.gid;
            value["data"]["name"] = group.name;
            value["data"]["introduction"] = group.introduction;
            edata->sendResponse(Json::FastWriter().write(value));
        } 
    }
}
//2成员
void Service::groupMeberList(std::string data,EpollData *edata)
{
    Json::Value value;
    Json::Reader reader;
    if(reader.parse(data, value))
    {
        auto gid = value["gid"].asInt();
        auto ret = mysql_->selectGroupShip(gid,2);
        std::list<GroupShip>::iterator it1;
        value["method"] = "responseGroupMeberList";
        value["data"] = Json::objectValue;
        int num = 0;
        for(it1 = ret.begin(); it1!=ret.end();it1++)
        {
            value["data"][num++] = (*it1).uid;
        }
        edata->sendResponse(Json::FastWriter().write(value));
    }
}
void Service::upload(std::string data,EpollData *edata)
{
    Json::Value value;
    Json::Reader reader;
    if(reader.parse(data, value))
    {
        auto number = value["number"].asInt();
        auto size = value["size"].asInt();
        auto fileName = value["fileName"].asString();
        if(number == 0)
        {
            
            int fd = open(fileName.data(),O_WRONLY);
            liveMap->fileMap[fileName] = fd;
        }
        auto buff = value["buff"].asString();
        auto fileFd = liveMap->fileMap[fileName];
        write(fileFd,buff.data(),buff.length());
        if(number+1*1024>=size)
        {
            close(liveMap->fileMap[fileName]);
            value = Json::objectValue;
            value["method"] = "responseUpload";
            value["data"] = fileName;
            edata->sendResponse(Json::FastWriter().write(value));
        }
        
    }
}

void Service::download(std::string data,EpollData *edata)
{
    Json::Value value;
    Json::Reader reader;
    if(reader.parse(data, value))
    {
        auto fileName = value["fileName"].asString();
        //struct stat statbuf; 
        //stat(fileName.data(),&statbuf); 
        //int size = statbuf.st_size;
        //while(size>1024)
        //{
        //    size-=1024;
        //} 
    }
};
void Service::addFriend(std::string data,EpollData *edata)
{
    Json::Value value;
    Json::Reader reader;
    Friends f;
    if(reader.parse(data, value))
    {
        f.frist_id = value["send_id"].asInt();
        f.second_id = value["recv_id"].asInt();
        f.status = 0;
    }
    mysql_->insertFriend(f);
}
void Service::addGroup(std::string data,EpollData *edata)
{

}
void Service::createGroup(std::string data,EpollData *edata)
{

}

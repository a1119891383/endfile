#ifndef LIVEMAP_H
#define LIVEMAP_H

#include<mutex>
#include<string>
#include<map>

#include"EpollData.h"

struct LiveMap{

    std::map<int,EpollData *> liveMap;
    std::map<std::string,int> fileMap;
    std::mutex lMutex;
    bool insertUser(int uid,EpollData *);
    bool deleteUser(EpollData* ptr);
    EpollData *findUser(int uid);
};

#endif
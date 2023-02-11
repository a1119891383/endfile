#include"EpollData.h"
#include"CoverUtil.h"
#include"Epollor.h"
#include<iostream>
EpollData::EpollData(int fd,Epollor *parent) : _fd(fd), _leave(false),_parent(parent){};

bool EpollData::sendResponse(std::string response)
{
    if(response.length()<=0)return false;
    std::lock_guard<std::mutex> lock(_outMutex);
    char len[4];
    CoverUtil::intToChars(response.length(),len);
    
    for(int i = 0; i < 4; ++i){
        _outBuff.push_back(len[i]);
    }
    _outBuff.append(response);
    //std::cout <<"_outBuff:" <<_outBuff << std::endl;
    _parent->toWrite(this);
    return true;
}
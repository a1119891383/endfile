#ifndef EPOLLDATA_H
#define EPOLLDATA_H
#include <string>
#include <mutex>
class Epollor;
class EpollData
{
public:
    EpollData(int fd,Epollor *parent);
    int _uid;
    int _fd;     //保存的文件描述符 
    bool _leave; //是否有剩余的数据存在
    std::string _inBuff;
    std::string _outBuff;
    std::mutex _inMutex;
    std::mutex _outMutex;
    Epollor *_parent;
    bool sendResponse(std::string response);
};
#endif

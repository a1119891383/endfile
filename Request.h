#ifndef REQUEST_H
#define REQUEST_H

#include <string>
#include"EpollData.h"
class Request
{
public:
    Request(std::string data,EpollData *ptr);
    ~Request();
    std::string _data;
    EpollData *_epollData;
};

#endif
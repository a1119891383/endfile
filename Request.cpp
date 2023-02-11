#include"Request.h"

Request::Request(std::string data,EpollData *ptr)
{
    _data = data;
    _epollData = ptr;
}
Request::~Request()
{
}
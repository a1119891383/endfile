#ifndef EPOLLOR_H
#define EPOLLOR_H

#include<string>
#include <map>
#include <mutex>
#include <unistd.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include"MsgQueue.h"
#include"EpollData.h"
#include"Request.h"

typedef unsigned int events;

class Epollor
{
public:
    Epollor();
    ~Epollor();
    void addEvent(int fd, bool nonblock = true);
    void toWrite(EpollData *);
    void run();
    bool _exit;
    MsgQueue<Request *> *_requestQueue;
    void updateEvent(EpollData *, events event);
    void deleteEvent(int fd);
    // uid - (fd,void *)
    std::mutex _cmutex;
    int _efd;
    int _connectionfd;
    std::mutex _emutex;
};
#endif
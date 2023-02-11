#ifndef ACCEPT_H
#define ACCEPT_H
#include <string>
#include "Epollor.h"

struct Accept
{
    Accept(Epollor *);
    ~Accept();
    /*
     *  在指定ip和hort上监听接受连接
     *
     */
    void listenAt(std::string ip, int port);
    Epollor *getEpollor() const;
    bool _exit;
private:
    void accpetLoop();
    int _fd;
    
    Epollor *_epollor = NULL;
};

#endif

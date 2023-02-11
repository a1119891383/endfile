#include "Accept.h"

#include <cstring>
#include <cstdio>

#include <netinet/in.h>
#include <arpa/inet.h>

#include<iostream>

#define MAX_ACCPET 100
#define IP_SIZE 20
void Accept::listenAt(std::string ip, int port)
{
    int rfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serveraddr;
    bzero(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(port);
    serveraddr.sin_addr.s_addr = inet_addr(ip.data());
    if (rfd == -1)
    {
        perror("socket");
        exit(1);
    }
    if ((bind(rfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr))) != 0)
    {
        perror("bind");
        exit(1);
    };
    ::listen(rfd, MAX_ACCPET);
    _fd = rfd;
    accpetLoop();
}
void Accept::accpetLoop()
{
    struct sockaddr_in clientAddr
    {
    };
    socklen_t socketlen = sizeof(struct sockaddr_in);
    char ip[IP_SIZE] = {0};
    int clientFd = -1;
    std::cout<< "开始监听连接" <<std::endl;
    while (!_exit)
    {
        clientFd = ::accept(_fd, (struct sockaddr *)&clientAddr, &socketlen);
        if (clientFd < 0)
        {
            //perror("connect error\n");
            continue;
        }
        std::cout<< "新连接："<< clientFd << std::endl;
        _epollor->addEvent(clientFd);
    }
}
Accept::Accept(Epollor *epollor)
{
    _exit = false;
    _epollor = epollor;
};
Accept::~Accept()
{
    _exit = true;
    sleep(1);

    close(_fd);
}
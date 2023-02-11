#include<thread>
#include<string>
#include"Accept.h"
#include"Epollor.h"
#include"MsgQueue.h"
#include"Request.h"
#include"Service.h"
#include"LiveMap.h"
MsgQueue<Request *> requestQ;
LiveMap liveMap;
int main(){
    Epollor *epollor = new Epollor();//数据读写
    epollor->_requestQueue = &requestQ;
    Service *service = new Service(&requestQ);
    service->liveMap = &liveMap;
    std::thread t1([=](){
        epollor->run();
    });//epoll进程运行
    std::thread t2([=](){
        service->run();
    });//service进程运行
    Accept *accept = new Accept(epollor);
    accept->listenAt("172.21.25.134", 8081);//监听连接
    //接受信号停止
    accept->_exit = true;
    epollor->_exit = true;
    service->_exit =true;
    t1.join();
    t2.join();
    return 0;
}
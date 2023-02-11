#include "Epollor.h"

#include <cstring>
#include<iostream>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include"EpollData.h"
#include"FileUtil.h"
#include"CoverUtil.h"
#include"json/json.h"

#define EVENTS_SIZE_MAX 20
#define READ_BUFF_SIZE_MAX 4096
#define MAX_ACCPET 20
Epollor::Epollor()
{
    _efd = ::epoll_create(1);
}
Epollor::~Epollor()
{
    _exit = true;
    //等待工作结束
   
}
void Epollor::run()
{
    struct epoll_event events[EVENTS_SIZE_MAX];
    int eNum;
    std::cout << "epoller 开始运行" <<std::endl;
    while (!_exit)
    {
        {
            std::lock_guard<std::mutex> lock(_emutex);
            eNum = epoll_wait(_efd, events, EVENTS_SIZE_MAX, 1);
        }
        for (int i = 0; i < eNum; ++i)
        {
            //std::cout << "有读写事件" << std::endl;
            if (events[i].events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR))
            { //异常
                //std::cout << "有异常事件" << std::endl;
                //移除在线队列
                Json::Value value;
                value == Json::objectValue;
                value["method"] ="errorExit";
                value["data"] = "";
                std::string ret = Json::FastWriter().write(value);
                // std::cout << "异常处理：" << ret << std::endl; 
                auto pt = (EpollData *)(events[i].data.ptr);
                Request *request = new Request(ret,pt);
                _requestQueue->push_msg(request);
                //删除Epoll事件
                int dfd = ((EpollData *)events[i].data.ptr)->_fd;
                std::cout << "释放：" << dfd << std::endl; 
                deleteEvent(dfd);
                //释放空间
                close(dfd);
                delete (EpollData *)events[i].data.ptr;
            }
            else if (events[i].events & EPOLLIN)//正常读
            {
                char buff[READ_BUFF_SIZE_MAX];
                int size = 0;
                void* pot = events[i].data.ptr;
                EpollData* eptr = (EpollData *)pot;
                int nfd = eptr->_fd;
                std::cout << "读事件文件描述符"  << nfd << std::endl;
                while ((size = ::read(nfd, buff, READ_BUFF_SIZE_MAX)) > 0)
                {
                    std::string buf;
                    
                    for (int i = 0; i < size; i++)
                    {
                        buf.push_back(buff[i]);
                    }

                    eptr->_inBuff.append(buf);
                }
                //std::cout << "inBuff内容：" << eptr->_inBuff << std::endl;
                int buffLen = eptr->_inBuff.length();
                //std::cout << "buffLen:"<<buffLen<<std::endl;
                while(buffLen>4)
                {
                    int msgLen = CoverUtil::charsToInt(eptr->_inBuff.data());
                    //std::cout << "msgLen:" << msgLen << std::endl;
                    if(buffLen>=msgLen+4)
                    {
                        
                        eptr->_inBuff.erase(0,4);
                        std::string temp = ((EpollData *)events[i].data.ptr)->_inBuff.substr(0,msgLen);
                        
                        std::cout << "消息入队:" << temp << std::endl;
                        eptr->_inBuff.erase(0,msgLen);
                        //封装request请求给业务处理层
                        Request *request = new Request(temp,eptr);
                        _requestQueue->push_msg(request);
                        //std::cout << "消息入队success:" << std::endl;
                        //test - start
                        //eptr->sendResponse(temp);
                        //test - end
                    }
                    else
                    {
                        break;
                    }    
                }
                //std::cout << "读结束" << std::endl;
                updateEvent(eptr,EPOLLIN | EPOLLRDHUP);
            }
            else if (events[i].events & EPOLLOUT)//正常写
            {
                void* pot = events[i].data.ptr;
                EpollData* eptr = (EpollData *)pot;
                int nfd = eptr->_fd;
                //std::cout << "触发写事件的文件描述符:" << nfd << std::endl;
                //std::cout<<"need to write:"<<eptr->_outBuff<<std::endl;
                //总共需要写出的字节数
                int needWrite = eptr->_outBuff.length();
                //std::cout<<"need to write:"<<needWrite<<std::endl;
                int writeNum = 0;
                int leaveNum = needWrite;
                while (leaveNum > 0)
                {
                    //已经写出的字节数
                    int ret = ::write(eptr->_fd, eptr->_outBuff.data() + writeNum, leaveNum);
                    //std::cout<<"writed:"<<ret<<std::endl;
                    //异常处理
                    if (ret < 0)
                    {
                        if (errno == EINTR)
                        {
                            continue;
                        }
                    }
                    writeNum += ret;
                    //还需要写出的字节数
                    leaveNum = needWrite - writeNum;
                    //std::cout<<"leaverNum:"<<leaveNum<<std::endl;
                }
                //写完清空outBuff
                eptr->_outBuff.clear();
                updateEvent(eptr,EPOLLIN | EPOLLRDHUP);
                //events[i].events = EPOLLIN | EPOLLRDHUP;
            }
        }
    }
}

void Epollor::updateEvent(EpollData *data, events event)
{
    struct epoll_event ev;
    ev.events = event;
    ev.data.ptr = data;
    {
        std::lock_guard<std::mutex> lock(_emutex);
        epoll_ctl(_efd, EPOLL_CTL_MOD, data->_fd, &ev);
    }
}
void Epollor::deleteEvent(int fd)
{
    {
        std::lock_guard<std::mutex> lock(_emutex);
        epoll_ctl(_efd, EPOLL_CTL_DEL, fd, NULL);
    }
    //顺便释放fd
    close(fd);
}
void Epollor::addEvent(int fd, bool nonblock)
{
    FileUntil::setNonBlock(fd);
    struct epoll_event event;
    memset(&event, 0, sizeof(event));
    event.events = EPOLLIN | EPOLLRDHUP;
    EpollData *eptr = new EpollData(fd,this);
    event.data.ptr = (void *)eptr;
    {
        std::lock_guard<std::mutex> lock(_emutex);
        int ret = ::epoll_ctl(_efd, EPOLL_CTL_ADD, fd, &event);
        std::cout << "注册epoll读写" <<std::endl;
    }
}

void Epollor::toWrite(EpollData *data)
{
    //updateEvent(fd, EPOLLOUT | EPOLLIN | EPOLLET);
    updateEvent(data, EPOLLOUT | EPOLLRDHUP);
}

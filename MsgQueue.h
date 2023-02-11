//消息队列
#ifndef MSGQUEUE_H
#define MSGQUEUE_H
#include <queue>
#include <mutex>
#include <condition_variable>
#include<iostream>
#define MSG_SIZE_MAX 100
template <typename T>
class MsgQueue
{
public:
    MsgQueue();
    ~MsgQueue();
    void push_msg(T t);
    T get_msg();
    void clear_msg();

private:
    std::mutex msg_mutex_;      //信号量
    std::queue<T> msg_queue_; //消息队列
    std::condition_variable condition_empty_;
    bool empty_;
};
template <typename T>
MsgQueue<T>::MsgQueue() :empty_(true){};
template <typename T>
MsgQueue<T>::~MsgQueue()
{
    clear_msg();
}
template <typename T>
void MsgQueue<T>::push_msg(T t)
{
    {
        std::unique_lock<std::mutex> lock(this->msg_mutex_);
        msg_queue_.push(t);
        empty_ = false;
        //std::cout << "msg push success" << std::endl;
    }
    condition_empty_.notify_all();
    //std::cout << "get up" << std::endl;
}
template <typename T>
T MsgQueue<T>::get_msg()
{
    //std::cout << "try to get msg" << std::endl;
    std::unique_lock<std::mutex> lock(this->msg_mutex_);
    while(empty_){
        condition_empty_.wait(lock);
    }
    auto ret = msg_queue_.front();
    //std::cout << "getted msg" << std::endl;
    msg_queue_.pop();
    if(msg_queue_.size()<=0)empty_ = true;
    return ret;
}

template <typename T>
void MsgQueue<T>::clear_msg()
{
    std::unique_lock<std::mutex> lock(this->msg_mutex_);
    while (!msg_queue_.empty())
    {
        msg_queue_.pop();
    }
}



#endif
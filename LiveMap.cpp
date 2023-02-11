#include"LiveMap.h"
#include"Epollor.h"
bool LiveMap::insertUser(int uid,EpollData *ptr)
{
    if(ptr == NULL) return false;
    std::lock_guard<std::mutex> lock(this->lMutex);
    liveMap[uid] = ptr;
    ptr->_uid = uid;
    std::cout << "用户：" << uid <<"注册至在线列表";
    return true;
}
EpollData *LiveMap::findUser(int uid)
{
    std::lock_guard<std::mutex> lock(this->lMutex);
    if(liveMap.find(uid)==liveMap.end())return NULL;
    return liveMap[uid];
}
bool LiveMap::deleteUser(EpollData* ptr)
{
    int uid = ptr->_uid;
    std::cout << "离线用户：" << uid << std::endl;
    std::lock_guard<std::mutex> lock(this->lMutex);
    if(liveMap.find(uid)!=liveMap.end())
    {
        auto t = liveMap[uid];
        t->_parent->deleteEvent(t->_fd);
        liveMap.erase(uid);
        std::cout << "成功移除：" << uid <<std::endl;
        delete t;
    }
    return true;
    
}
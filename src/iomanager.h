#ifndef __SYLAR_IOMANAGER_H__
#define __SYLAR_IOMANAGER_H__

#include "scheduler.h"

namespace sylar{

class IOManager : public Scheduler {
public:
    typedef std::shared_ptr<IOManager> ptr;
    typedef RWMutex RWMutexType;

    enum Event {
        //无事件
        NONE = 0x0;
        //读事件(EPOLLIN)
        READ = 0x1;
        //写事件(EPOLLOUT)
        WRITE = 0x4;
    };



private:
    //socket事件上下文类
    struct FdContext {
        typedef Mutex MutexType;
        struct EventContext {
            Scheduler* scheduler = nullptr;     //事件执行的scheduler
            Fiber::ptr fiber;                   //事件协程
            std::function<void()> cb;           //事件的回调函数
        };
        //返回对应事件的上下文
        EventContext& getContext(Event event);
        //重置事件上下文
        void resetContext(EventContext& ctx);
        //触发事件
        void triggerEvent(Event event);

        EventContext read;           //读事件上下文
        EventContext write;          //写事件上下文
        int fd;                      //事件关联的句柄
        Event m_events = NONE;       //已经注册的事件
        MutexType mutex;
    }

public:
    IOManager(size_t threads = -1, bool use_caller = true, const std::string& name = "");
    ~IOManager();

    int addEvent(int fd, Event event, std::function<void()> cb = nullptr);
    bool delEvent(int fd, Event event);
    bool cancelEvent(int fd, Event event);

    bool cancelAll(int fd);

    static IOManager* GetThis();

protected:
    void tickle() override;
    bool stopping() override;
    void idle() override;

    //重置socket句柄上下文的容器大小
    void contextResize(size_t size);
private:
    int m_epfd = 0;
    int m_tickleFds[2];

    //等待执行事件的数量
    std::atomic<size_t> m_pendingEventCount = {0};
    RWMutexType m_mutex;
    std::vector<FdContext*> m_fdContexts;
};

}



#endif
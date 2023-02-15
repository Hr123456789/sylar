#ifndef __SYLAR_SCHEDULER_H__
#define __SYLAR_SCHEDULER_H__

#include <memory>
#include "fiber.h"
#include "thread.h"
#include <vector>
#include <list>

namespace sylar{

class Scheduler {
public:
    typedef std::shared_ptr<Scheduler> ptr;
    typedef Mutex MutexType;

    //线程数量、是否使用当前调用线程、协程调度器名称
    Scheduler(size_t threads = 1, bool use_caller  = true, const std::string& name = "");
    virtual ~Scheduler();

    const std::string& getName() const {return m_name;}

    //返回当前协程调度器
    static Scheduler* GetThis();
    //返回当前协程调度器的调度协程
    static Fiber* GetMainFiber();

    void start();
    void stop();

    template<class FiberOrCb>
    void schedule(FiberOrCb fc, int thread = -1) {
        bool need_tickle = false;
        {
            MutexType::Lock lock(m_mutex);
            need_tickle = scheduleNoLock(fc, thread);
        }

        if(need_tickle) {
            //通知
            tickle();
        }
    }

    //批量添加
    template<class InputIterator>
    void schedule(InputIterator begin, InputIterator end) {
        bool need_tickle = false;
        {
            MutexType::Lock lock(m_mutex);
            while(begin != end) {
                need_tickle = scheduleNoLock(&*begin) || need_tickle;
                ++begin;
            }
        }
        if(need_tickle) {
            tickle();
        }
    }

    void switchTo(int thread = -1);
    std::ostream& dump(std::ostream& os);
protected:
    //通知协程调度器有任务了
    virtual void tickle();
    void run();
    //返回是否可以停止
    virtual bool stopping();
    //空闲协程
    virtual void idle();
    //设置当前的协程调度器
    void setThis();

    bool hasIdleThreads() const {return m_idleThreadCount > 0;}
private:
    template<class FiberOrCb>
    bool scheduleNoLock(FiberOrCb fc, int thread) {
        //是否需要通知
        bool need_tickle = m_fibers.empty();
        FiberAndThread ft(fc, thread);
        if(ft.fiber || ft.cb) {
            m_fibers.push_back(ft);
        }
        return need_tickle;
    }
private:
    struct FiberAndThread {
        Fiber::ptr fiber;
        std::function<void()> cb;
        //线程ID
        int thread;

        FiberAndThread(Fiber::ptr f, int thr):fiber(f),thread(thr) {

        }
        FiberAndThread(Fiber::ptr* f,int thr):thread(thr) {
            fiber.swap(*f);
        }

        FiberAndThread(std::function<void()> f, int thr):cb(f),thread(thr) {
        }

        FiberAndThread(std::function<void()>* f, int thr):thread(thr) {
            cb.swap(*f);
        }

        FiberAndThread():thread(-1) {}

        void reset() {
            fiber = nullptr;
            cb = nullptr;
            thread = -1;
        }
    };
    
private:
    MutexType m_mutex;
    //线程池
    std::vector<Thread::ptr> m_threads;
    //要执行的协程
    std::list<FiberAndThread> m_fibers;
    Fiber::ptr m_rootFiber;
    std::string m_name;

protected:
    //线程ID
    std::vector<int>m_threadIds;
    size_t m_threadCount = 0;
    //活跃
    size_t m_activeThreadCount = 0;
    //空闲
    size_t m_idleThreadCount = 0;
    bool m_stopping = true;
    bool m_autoStop = false;
    //主线程ID
    int m_rootThread = 0;
};



}


#endif
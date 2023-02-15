#ifndef __SYLAR_FIBER_H__
#define __SYLAR_FIBER_H__

#include <memory>
#include <functional>
#include <ucontext.h>

namespace sylar{
class Scheduler;
class Fiber : public std::enable_shared_from_this<Fiber> {
friend class Scheduler;
public:
    typedef std::shared_ptr<Fiber> ptr;

    enum State {
        //初始状态
        INIT,
        //暂停状态
        HOLD,
        //执行中状态
        EXEC,
        //结束状态
        TERM,
        //可执行状态
        READY,
        //异常状态
        EXCEPT
    };

private:
    Fiber();

public:
    Fiber(std::function<void()> cb, size_t stacksize = 0);
    ~Fiber();
    //重置协程函数，并重置状态
    void reset(std::function<void()> cb);
    //切换到当前协程执行
    void swapIn();
    //协程切换到后台
    void swapOut();

    //当前线程切换到执行状态，执行的为当前线程的主协程
    void call();

    //将当前线程切换到后台，执行的为该协程，返回到线程的主协程
    void back();

    uint64_t getId() const {return m_id;}
    State getState() const {return m_state;}

public:
    //设置当前协程
    static void SetThis(Fiber* f);
    //返回当前协程
    static Fiber::ptr GetThis();
    //协程切换到后台，并设置为Ready状态
    static void YieldToReady();
    //协程切换到后台，并设置为Hold状态
    static void YieldToHold();

    static uint64_t TotalFibers();
    static void MainFunc();
    //协程执行函数，执行完成返回到线程调度协程
    static void CallerMainFunc();
    static uint64_t GetFiberId();

private:
    uint64_t m_id = 0;
    //栈大小
    uint32_t m_stacksize = 0;
    //状态
    State m_state = INIT;

    ucontext_t m_ctx;
    void* m_stack = nullptr;

    std::function<void()> m_cb;

};

}

#endif
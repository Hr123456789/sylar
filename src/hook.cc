#include "hook.h"
#include <dlfcn.h>


#include "config.h"
#include "log.h"
#include "fiber.h"
#include "iomanager.h"
#include "macro.h"


namespace sylar {
static thread_local bool t_hook_enable = false;


#define HOOK_FUN(XX) \
    XX(sleep) \
    XX(usleep) \

void hook_init() {
    static bool is_inited = false;
    if(is_inited) {
        return;
    }
}

//dlsym函数的功能就是可以从共享库（动态库）中获取符号（全局变量与函数符号）地址，
//通常用于获取函数符号地址，这样可用于对共享库中函数的包装；
#define XX(name) name ## _f = (name ## _fun)dlsym(RTLD_NEXT, #name);
    HOOK_FUN(XX);
//#undef 标识符，用来将前面定义的宏标识符取消定义。
#undef XX


bool is_hook_enable() {
    return t_hook_enable;
}

void set_hook_enable(bool flag) {
    t_hook_enable = flag;
}


extern "C" {
#define XX(name) name ## _fun name ## _f = nullptr;
    HOOK_FUN(XX);
#undef XX

unsigned int sleep(unsigned int seconds) {
    //如果不采用HOOK就调用原函数
    if(!sylar::t_hook_enable) {
        return sleep_f(seconds);
    }

    //使用HOOK
    sylar::Fiber::ptr fiber = sylar::Fiber::GetThis();
    sylar::IOManager* iom = sylar::IOManager::GetThis();
    iom->addTimer(seconds * 1000, std::bind((void(sylar::Scheduler::*)
            (sylar::Fiber::ptr, int thread))&sylar::IOManager::schedule
            ,iom, fiber, -1));
    sylar::Fiber::YieldToHold();
    return 0;
}

}

}
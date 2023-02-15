#ifndef __SYLAR_UTIL_H__
#define __SYLAR_UTIL_H__

#include <pthread.h>
#include <stdint.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <string>
#include "log.h"
#include <vector>
#include <sys/time.h>
namespace sylar{

//返回当前线程的ID
pid_t GetThreadId();

//返回当前的协程号
uint32_t GetFiberId();

void Backtrace(std::vector<std::string>& bt, int size = 64, int skip = 1);

std::string BacktraceToString(int size = 64, int skip = 2, const std::string& prefix = "");

//时间ms
uint64_t GetCurrentMS();
uint64_t GetCurrentUS();

}


#endif
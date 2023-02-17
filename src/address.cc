#include "address.h"
#include "log.h"
#include <sstream>
#include <netdb.h>
#include <ifaddrs.h>
#include <stddef.h>

namespace sylar {
static sylar::Logger::ptr g_loger = SYLAR_LOG_NAME("system");

//没看懂
template<class T>
static T CreateMask(uint32_t bits) {
    return (1 << (sizeof(T) * 8 - bits)) - 1;
}

template<class T>
static uint32_t CountBytes(T value) {
    uint32_t result = 0;
    for(;value; ++result) {
        value &= value - 1;
    }
    return result;
}
}
#include "../src/fiber.h"
#include "../src/log.h"
#include "../src/util.h"

sylar::Logger::ptr g_logger = SYLAR_LOG_ROOT();

void run_in_fiber() {
    SYLAR_LOG_INFO(g_logger) << "run_in_fiber begin";
    sylar::Fiber::YieldToHold();
    SYLAR_LOG_INFO(g_logger) << "run_in_fiber end";
    sylar::Fiber::YieldToHold();
}

int main() {
    sylar::Fiber::GetThis();
    SYLAR_LOG_INFO(g_logger) << "main begin";
    sylar::Fiber::ptr fiber(new sylar::Fiber(run_in_fiber,128 * 1024));
    fiber->swapIn();
    SYLAR_LOG_INFO(g_logger) << "main after swapIn";
    fiber->swapIn();
    SYLAR_LOG_INFO(g_logger) << "main after end";
    return 0;
}
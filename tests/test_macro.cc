#include "../src/log.h"
#include <assert.h>
#include "../src/util.h"

sylar::Logger::ptr g_logger = SYLAR_LOG_ROOT();

void test_assert() {
    SYLAR_LOG_INFO(g_logger) << sylar::BacktraceToString(10, 0, "    ");
}

int main() {
    test_assert();
    return 0;
}
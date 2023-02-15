#include "../src/log.h"
#include "../src/util.h"
#include <iostream>

int main()
{
    sylar::Logger::ptr logger(new sylar::Logger);
    sylar::StdoutLogAppender::ptr stdout_appender(new sylar::StdoutLogAppender);
    logger->addAppender(stdout_appender);
    sylar::FileLogAppender::ptr file_appender(new sylar::FileLogAppender("./log.txt"));
    sylar::LogFormatter::ptr fmt(new sylar::LogFormatter("%d%T%p%T%m%n"));
    file_appender->setFormatter(fmt);
    //stdout_appender->setFormatter(fmt);
    file_appender->setLevel(sylar::LogLevel::ERROR);
    logger->addAppender(file_appender);

    sylar::LogEvent::ptr event(new sylar::LogEvent(logger,sylar::LogLevel::DEBUG,__FILE__, __LINE__, 0, sylar::GetThreadId(), sylar::GetFiberId(), time(0),"hr"));
    event->getSS() << "hello sylar log";
    logger->log(sylar::LogLevel::DEBUG, event);

    SYLAR_LOG_INFO(logger) << "test macro";
    SYLAR_LOG_ERROR(logger) << "test macro error";

    SYLAR_LOG_FMT_ERROR(logger, "test macro fmt error %s", "aa");

    auto l = sylar::LoggerMgr::GetInstance()->getLogger("xx");
    SYLAR_LOG_INFO(l) << "xxx";
    return 0;
}
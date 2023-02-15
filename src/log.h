#ifndef __SYLAR_LOG_H__
#define __SYLAR_LOG_H__

#include <string>
#include <stdint.h>
#include <memory>
#include <list>
#include <sstream>
#include <fstream>
#include <vector>
#include <iostream>
#include <map>
#include <stdarg.h>   //va_list
#include "singleton.h"
//使用流方式将日志级别level的日志写入到logger
#define SYLAR_LOG_LEVEL(logger, level) \
    if(logger->getLevel() <= level) \
        sylar::LogEventWrap(sylar::LogEvent::ptr(new sylar::LogEvent(logger, level, \
                         __FILE__, __LINE__, 0, sylar::GetThreadId(), \
                         sylar::GetFiberId(), time(0),"hr"))).getSS()

//使用流方式将日志级别debug的日志写入到logger
#define SYLAR_LOG_DEBUG(logger) SYLAR_LOG_LEVEL(logger, sylar::LogLevel::DEBUG)

//使用流方式将日志级别info的日志写入到logger
#define SYLAR_LOG_INFO(logger) SYLAR_LOG_LEVEL(logger, sylar::LogLevel::INFO)

//使用流方式将日志级别warn的日志写入到logger
#define SYLAR_LOG_WARN(logger) SYLAR_LOG_LEVEL(logger, sylar::LogLevel::WARN)

//使用流方式将日志级别error的日志写入到logger
#define SYLAR_LOG_ERROR(logger) SYLAR_LOG_LEVEL(logger, sylar::LogLevel::ERROR)

//使用流方式将日志级别fatal的日志写入到logger
#define SYLAR_LOG_FATAL(logger) SYLAR_LOG_LEVEL(logger, sylar::LogLevel::FATAL)

//使用格式化方式将日志级别为level的日志写入到logger
#define SYLAR_LOG_FMT_LEVEL(logger, level, fmt, ...) \
    if(logger->getLevel() <= level) \
        sylar::LogEventWrap(sylar::LogEvent::ptr(new sylar::LogEvent(logger, level, \
                            __FILE__, __LINE__, 0, sylar::GetThreadId(), \
                            sylar::GetFiberId(), time(0), "hr"))).getEvent()->format(fmt,  __VA_ARGS__)

//使用格式化方式将日志级别为debug的日志写入到logger
#define SYLAR_LOG_FMT_DEBUG(logger,fmt, ...) SYLAR_LOG_FMT_LEVEL(logger, sylar::LogLevel::DEBUG, fmt, __VA_ARGS__) 

//使用格式化方式将日志级别为info的日志写入到logger
#define SYLAR_LOG_FMT_INFO(logger,fmt, ...) SYLAR_LOG_FMT_LEVEL(logger, sylar::LogLevel::INFO, fmt, __VA_ARGS__)

//使用格式化方式将日志级别为warn的日志写入到logger
#define SYLAR_LOG_FMT_WARN(logger,fmt, ...) SYLAR_LOG_FMT_LEVEL(logger, sylar::LogLevel::WARN, fmt, __VA_ARGS__)

//使用格式化方式将日志级别为error的日志写入到logger
#define SYLAR_LOG_FMT_ERROR(logger,fmt, ...) SYLAR_LOG_FMT_LEVEL(logger, sylar::LogLevel::ERROR, fmt, __VA_ARGS__)

//使用格式化方式将日志级别为fatal的日志写入到logger
#define SYLAR_LOG_FMT_FATAL(logger,fmt, ...) SYLAR_LOG_FMT_LEVEL(logger, sylar::LogLevel::FATAL, fmt, __VA_ARGS__)

//获取主日志器
#define SYLAR_LOG_ROOT() sylar::LoggerMgr::GetInstance()->getRoot()
//获取name的日志器
#define SYLAR_LOG_NAME(name) sylar::LoggerMgr::GetInstance()->getLogger(name)
namespace sylar {
class Logger;
class LoggerManager;
//日志级别
class LogLevel {
public:
    enum Level {
        UNKNOW = 0,
        DEBUG = 1,
        INFO = 2,
        WARN = 3,
        ERROR = 4,
        FATAL = 5
    };

    //将日志级别转换成文本输出
    static const char* ToString(LogLevel::Level level);
    //将文本转换成日志级别
    static LogLevel::Level fromString(const std::string& str);
};

//日志事件
class LogEvent {
public:
    typedef std::shared_ptr<LogEvent> ptr;
    LogEvent(std::shared_ptr<Logger> logger, LogLevel::Level level
            ,const char* file, int32_t line, uint32_t elapse
            ,uint32_t thread_id, uint32_t fiber_id, uint64_t time
            ,const std::string& thread_name);

    //返回文件名
    const char* getFile() const {return m_file;}
    //返回行号
    int32_t getLine() const {return m_line;}
    //返回耗时
    uint32_t getElapse() const {return m_elapse;}
    //返回线程ID
    uint32_t getThreadId() const {return m_threadId;}
    //返回协程ID
    uint32_t getFiberId() const {return m_fiberId;}
    //返回时间
    uint64_t getTime() const {return m_time;}
    //返回线程名称
    const std::string& getThreadName() const {return m_threadName;}
    //返回日志内容
    std::string getContent() const {return m_ss.str();}
    //返回日志器
    std::shared_ptr<Logger> getLogger() const {return m_logger;}
    //返回日志级别
    LogLevel::Level getLevel() const {return m_level;}
    //返回日志字符串流
    std::stringstream& getSS() {return m_ss;}
    //格式化写入日志内容
    void format(const char* fmt, ...);
    //格式化写入日志内容
    void format(const char* fmt, va_list al);

private:
    const char* m_file = nullptr;     //文件名
    int32_t m_line = 0;              //行号
    uint32_t m_elapse = 0;            //程序启动开始到现在的毫秒数
    uint32_t m_threadId = 0;        //线程id
    uint32_t m_fiberId = 0;         //协程id
    uint64_t m_time = 0;            //时间戳
    std::string m_threadName;       //线程名称
    std::stringstream m_ss;         //日志内容流
    std::shared_ptr<Logger> m_logger; //日志器
    LogLevel::Level m_level;         //日志等级
};


//日志包装器
class LogEventWrap
{
public:
    LogEventWrap(LogEvent::ptr e);
    ~LogEventWrap();
    //获取日志事件
    LogEvent::ptr getEvent() const {return m_event;}
    //获取日志内容流
    std::stringstream& getSS();

private:
    LogEvent::ptr m_event;
};


//日志格式器
class LogFormatter {
public:
    typedef std::shared_ptr<LogFormatter> ptr;

    LogFormatter(const std::string& pattern);

    //格式化输出日志内容
    std::string format(std::shared_ptr<Logger>logger, LogLevel::Level level, LogEvent::ptr event);
    //流的方式输出日志内容
    std::ostream& format(std::ostream& ofs, std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event);
public:
    class FormatItem
    {
    public:
        typedef std::shared_ptr<FormatItem> ptr;

        virtual ~FormatItem() {}

        virtual void format(std::ostream& os, std::shared_ptr<Logger>logger, LogLevel::Level level, LogEvent::ptr event) = 0;

    };

    //初始化，解析日志模板
    void init();

    //是否有错误
    bool isError() const {return m_error;}
private:
    //日志模板
    std::string m_pattern;
    //日志解析后格式
    std::vector<FormatItem::ptr> m_items;
    //是否有错误
    bool m_error = false;
};

//日志输出地
class LogAppender {
friend class Logger;
public:
    typedef std::shared_ptr<LogAppender> ptr;
    virtual ~LogAppender() {}

    virtual void log(std::shared_ptr<Logger>logger, LogLevel::Level level, LogEvent::ptr event) = 0;
    //设置日志输出格式
    void setFormatter(LogFormatter::ptr val) {m_formatter = val;}
    //获取日志输出格式
    LogFormatter::ptr getFormatter() const {return m_formatter;}
    //获取日志级别
    LogLevel::Level getLevel() const {return m_level;}
    //设置日志级别
    void setLevel(LogLevel::Level level) {m_level = level;}
protected:
    LogLevel::Level m_level;
    //是否有自己的日志格式器
    bool m_hasFormatter = false;
    LogFormatter::ptr m_formatter;
};

//日志器
class Logger : public std::enable_shared_from_this<Logger> {
friend class LoggerManager;
public:
    typedef std::shared_ptr<Logger> ptr;

    Logger(const std::string& name = "root");
    void log(LogLevel::Level level, LogEvent::ptr event);

    void debug(LogEvent::ptr event);
    void info(LogEvent::ptr event);
    void warn(LogEvent::ptr event);
    void error(LogEvent::ptr event);
    void fatal(LogEvent::ptr event);

    //添加日志输出位置
    void addAppender(LogAppender::ptr appender);
    //删除日志输出
    void delAppender(LogAppender::ptr appender);
    //获取日志等级
    LogLevel::Level getLevel() const {return m_level;}
    //设置日志等级
    void setLevel(LogLevel::Level val) {m_level = val;}
    //清空日志输出目标
    void clearAppender();
    //返回日志名称
    const std::string& getName() const {return m_name;}
    //设置日志格式器
    void setFormatter(LogFormatter::ptr val);
    //设置日志格式模板
    void setFormatter(const std::string& val);
    //获取日志格式器
    LogFormatter::ptr getFormatter();
    
private:
    std::string m_name;
    LogLevel::Level m_level;
    std::list<LogAppender:: ptr> m_appenders;
    //日志格式器
    LogFormatter::ptr m_formatter;
    //主日志器
    Logger::ptr m_root;
};

//输出到控制台的Appender
class StdoutLogAppender : public LogAppender {
public:
    typedef std::shared_ptr<StdoutLogAppender> ptr;
    void log(std::shared_ptr<Logger>logger,LogLevel::Level level, LogEvent::ptr event) override;
};

//输出到文件的Appender
class FileLogAppender : public LogAppender {
public:
    typedef std::shared_ptr<FileLogAppender> ptr;
    FileLogAppender(const std::string& filename);
    void log(std::shared_ptr<Logger>logger,LogLevel::Level level, LogEvent::ptr event) override;

    //重新打开文件，文件打开成功返回true
    bool reopen();
private:
    std::string m_filename;
    std::ofstream m_filestream;
};

//日志器管理类
class LoggerManager
{
public:

    LoggerManager(); 
    //获取日志器
    Logger::ptr getLogger(const std::string& name);
    //初始化
    void init();
    //返回主日志器
    Logger::ptr getRoot() const {return m_root;}

private:
    //日志器容器
    std::map<std::string, Logger::ptr> m_loggers;
    //主日志器
    Logger::ptr m_root;
};

//日志器管理类单例模式
typedef sylar::Singleton<LoggerManager> LoggerMgr;

}
#endif
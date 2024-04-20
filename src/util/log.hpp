#ifndef LOG_H
#define LOG_H

#include "include.hpp"
#include "mytime.hpp"

#if WIN32
#define CLASS_METHOD __FUNCTION__
#else
inline std::string methodName(const std::string& prettyFunction)
{
    size_t colons = prettyFunction.find("::");
    size_t begin = prettyFunction.substr(0,colons).rfind(" ") + 1;
    size_t end = prettyFunction.rfind("(") - begin;
    
    return prettyFunction.substr(begin,end) + "()";
}

#define CLASS_METHOD methodName(__PRETTY_FUNCTION__)
#endif

#define LOG(level, message)   \
{\
std::ostringstream oss; \
oss << message; \
Logger::instance().log(level, CLASS_METHOD, __LINE__, oss);\
};

#define LOG_FUNCTION() \
LogFunction l(TraceFunction, CLASS_METHOD);

#define LOG_FUNCTION_INFO() \
LogFunction l(Info, CLASS_METHOD);

#define LOG_ASSERT(cond, message)  \
{\
if(!(cond)){ \
LOG(Warning_Assert, message) \
};\
};

#define ASSERT(cond, message)  \
{\
if(!(cond)){ \
LOG(Warning_Assert, message) \
};\
};

#define DEBUG(x) LOG(Trace0,x)
#define INFO(x) LOG(Info,x)
#define WARN(x) LOG(Warning,x)
#define ERROR(x) LOG(MyError,x)

enum Level{
    Trace0 = 0,
    TraceFunction = 1,
    TraceLock = 2,
    Trace = 3,
    Debug = 10,
    Info = 20,
    Todo = 30,
    StrategyLog = 35,
    TradeLog = 36,
    Warning = 40,
    Warning_Assert = 50,
    MyError = 60,
    Fatal = 70
};

class ILogger{
public:
    Level m_levelMin;
    
    ILogger(Level levelMin = Trace0)
    :m_levelMin(levelMin)
    {
    }
    virtual void log(Level level, const std::string & s) = 0;
};

class LogFile : public ILogger{
    std::ofstream m_logFile;
public:
    std::string m_path;
    
    LogFile(std::string path, Level levelMin);
    void log(Level level, const std::string & s);
};

class LogCout : public ILogger{
public:
    LogCout(Level levelMin);
    void log(Level level, const std::string & s);
};

class Logger {
public:
    void add(ILogger * logger);
    void removeLogger();
    
    void log(Level level, std::string method, int line, std::ostringstream & oss);
    static Logger & instance();
    
    void setLogLevelMinCout(Level l);

private:
    Level m_levelMin;
    LogFile m_logFile;
    LogCout m_logCout;
    ILogger * m_logger;
    
    QMutex m_mutex;
    
    void initLogger(ILogger & logger);
    void calculLevelMin();
    
    Logger();
    virtual ~Logger();
};

class LogFunction{
public:
    LogFunction(Level level, std::string function);
    virtual ~LogFunction();
private:
    myTime m_tickBegin;
    static myTime m_tickRef;
    
    Level m_level;
    std::string m_function;
    static int m_count;
};

#endif //LOG_H

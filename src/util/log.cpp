#include "log.hpp"
#include "util.hpp"
#include "environnement.hpp"
#include "directory_manager.hpp"
#include <iostream>
#include <cerrno>

LogFile::LogFile(std::string path, Level levelMin)
   :ILogger(levelMin)
{
    m_logFile.open(path);
   if(m_logFile.fail()){
       //std::cerr << "open failure as expected: " << strerror(errno) << std::endl;
       std::cerr << "Warning File fail " << path << std::endl;
       exit(1);
   }
}

void LogFile::log(Level level, const std::string & s){
   m_logFile << s << std::endl;
}


LogCout::LogCout(Level levelMin)
   :ILogger(levelMin)
{
}

void LogCout::log(Level level, const std::string & s){
   std::cout << s << std::endl;
}

Logger::Logger()
   :m_logFile(DirectoryManager::instance().m_log_file, Trace0) // ouverture en écriture avec effacement du fichier ouver
   ,m_logCout(Info)
   ,m_logger(NULL)
{
   calculLevelMin();
   initLogger(m_logCout);
   initLogger(m_logFile);
}

Logger & Logger::instance(){
    static Logger obj;
    return obj;
}

void Logger::initLogger(ILogger & logger){
   std::ostringstream oss2;
   oss2 << std::endl;
   oss2 << "#####################################" << std::endl;
   oss2 << "##                                 ##" << std::endl;
   oss2 << "##             begin log           ##" << std::endl;
   oss2 << "##                                 ##" << std::endl;
   oss2 << "#####################################" << std::endl;

   char buffer[256];
   time_t timestamp = time(NULL);

   strftime(buffer, sizeof(buffer), "%A %d %B %Y - %X.", localtime(&timestamp));
   oss2 << "## " << buffer  << std::endl;
   oss2 << "## " << ProjectCompleteName  << std::endl;
   oss2 << "## " << "machine : " << MachineName << std::endl;
   oss2 << "## " << "type of time : " << TYPE_OF_TIME << std::endl;
   oss2 << "#####################################" << std::endl;

   logger.log(Info, oss2.str());
}

void Logger::calculLevelMin(){
   m_levelMin = (Level)std::min((int)m_logCout.m_levelMin, (int)m_logFile.m_levelMin);
}

void Logger::setLogLevelMinCout(Level l){
    m_logCout.m_levelMin = l;
    calculLevelMin();
    
}

Logger::~Logger()
{
}

void Logger::add(ILogger* logger)
{
   m_logger = logger;
}

void Logger::removeLogger()
{
   QMutexLocker l(&m_mutex);
   m_logger = NULL;
}

void Logger::log(Level level, std::string method, int line, std::ostringstream & oss){
   if(level >= MyError){
      callDebugger();
   }

   if(level < m_levelMin){
      return;
   }

   std::ostringstream oss2;
   switch(level)
   {
      case Trace0 :
         oss2 << "Trace0  ";
         break;
      case TraceFunction :
         oss2 << "TraceF  ";
         break;
      case TraceLock :
         oss2 << "TraceLck";
         break;
      case Trace :
         oss2 << "Trace   ";
         break;
      case Debug :
         oss2 << "Debug   ";
         break;
      case Info :
         oss2 << "Info    ";
         break;
      case Todo :
         oss2 << "***Todo ";
         break;
      case TradeLog :
         oss2 << "Trade   ";
         break;
      case StrategyLog :
         oss2 << "Strat   ";
         break;
      case Warning :
         oss2 << "*Warn   ";
         break;
      case Warning_Assert :
         oss2 << "*WarnAs ";
         break;
      case MyError :
         oss2 << "**Error ";
         break;
      case Fatal :
         oss2 << "******* ";
         break;
   }
   
   oss2 << method << "(l. " << line << ")   " << oss.str();
   std::string res = oss2.str();

   if(level >= m_logCout.m_levelMin){
      m_logCout.log(level, res);
   }
   if(level >= m_logFile.m_levelMin){
      m_logFile.log(level, res);
   }

   if(m_logger != NULL && level >= m_logger->m_levelMin){
      QMutexLocker l(&m_mutex);
      m_logger->log(level, res);
   }
};



int LogFunction::m_count = 0;
myTime LogFunction::m_tickRef = myTimeInit();

LogFunction::LogFunction(Level level, std::string function)
   :m_tickBegin(myTimeInit()),
   m_level(level),
   m_function(function)
{
   ++m_count;
   std::string message;
   for(int i = 0; i< m_count; ++i){
      message += std::string("++");
   }
   message += " " + m_function;
   std::ostringstream oss;
   Logger::instance().log(level, message, 0, oss);
   //LOG(m_level, m_class_debug, message, "");
};

LogFunction::~LogFunction(){
   myTime endTick = myTimeInit();
   double duration = myTimeDurationMs(m_tickBegin, endTick);
   std::string message;
   for(int i = 0; i< m_count; ++i){
      message += std::string("--");
   }
   message += " " + m_function;
   std::ostringstream oss;
   oss << duration << " ms => " << myTimeDurationMs(m_tickRef, endTick) << " - " << myTimeDurationMs(m_tickRef, m_tickBegin);
   Logger::instance().log(m_level, message, 0, oss);
   --m_count;
};

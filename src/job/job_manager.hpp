#ifndef JOB_MANAGER_H
#define JOB_MANAGER_H

#include "../util/include.hpp"

class JobManager  {
    std::string m_begin;
    std::string m_file;
    
public:
    JobManager();

    void init();
    void handleSeconde();
    void handle60s();
    
    std::string m_log_path;
    std::ofstream m_log_file;
    
    std::string m_data_path;
    std::ofstream m_data_file;
    
    void log(const std::string & string);
    void addData(const std::string & string);
};

#endif

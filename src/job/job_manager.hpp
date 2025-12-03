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
};

#endif

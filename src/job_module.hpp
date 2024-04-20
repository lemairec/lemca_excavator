#ifndef JOB_MODULE_H
#define JOB_MODULE_H

#include "util/include.hpp"
#include "gps/position_module.hpp"

class JobModule {
    std::string m_file_info;
    std::string m_date_str;
    
    std::ofstream m_file_job_stream;
    std::ofstream m_file_record_stream;

public:
    JobModule();
    
    void saveInfoFile(double surface);
    void onNewImportantPoint(GpsPoint_ptr p);
    void onNewMessage(const std::string & s);
    void onNewFrame(const std::string & s);
};

#endif // GPS_FRAMEWORK_H

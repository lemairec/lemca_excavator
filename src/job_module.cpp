#include "job_module.hpp"
#include "util/directory_manager.hpp"
#include "util/log.hpp"

#include <QDateTime>

JobModule::JobModule(){
    DirectoryManager::instance().makeRelativeDir("/job");
    QDateTime date = QDateTime::currentDateTime();
    QString s = date.toString("yyyyMMddThhmm");
    m_date_str = s.toUtf8().constData();
    
    m_file_info = DirectoryManager::instance().getDataDirectory() + "/job/gps_" + s.toUtf8().constData() + ".info";
    
    //std::string file_job = DirectoryManager::instance().getDataDirectory() + "/job/gps_" + s.toUtf8().constData() + ".job";
    //m_file_job_stream.open(file_job, std::ios::out);
    
    //INFO(file_job);
    //INFO(m_file_job_stream.is_open());
    
    std::string file_record = DirectoryManager::instance().getDataDirectory() + "/job/gps_" + s.toUtf8().constData() + ".record";
    m_file_record_stream.open(file_record, std::ios::out);
}

void JobModule::saveInfoFile(double surface){
    std::ofstream file;
    file.open(m_file_info, std::ios::out);
    
    QDateTime date_end = QDateTime::currentDateTime();
    QString s = date_end.toString("yyyyMMddThhmm");
    std::string date_end_str = s.toUtf8().constData();
    
    
    file << "{" << std::endl;
    file << "\"date_begin\":\"" << m_date_str << "\"" << std::endl;
    file << ",\"date_end\":\"" << date_end_str << "\"" << std::endl;
    file << ",\"surface\":" << surface << std::endl;
    file << "}" << std::endl;
}

void JobModule::onNewImportantPoint(GpsPoint_ptr p){
    //m_file_job_stream << p->m_time << "," << std::setprecision(14) << p->m_latitude << "," << p->m_longitude << std::endl;
    
}


void JobModule::onNewMessage(const std::string & s){
    //m_file_job_stream << "[" << s << "]"<< std::endl;
}

void JobModule::onNewFrame(const std::string & s){
    //m_file_record_stream << s << std::endl;
}

/*
file_debug_stream.open(file_debug, std::ios::out);
//mode_t mt;

INFO(file_debug);
INFO(file_debug_stream.is_open());*/

///Users/lemairec/lemca_data/agrigpspi/job/test.record

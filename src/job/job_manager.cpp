#include "job_manager.hpp"
#include "../util/directory_manager.hpp"
#include "../util/log.hpp"
#include "../framework_vision.hpp"

JobManager::JobManager(){
    
}

void JobManager::init(){
    std::string dir = DirectoryManager::instance().getHome()+"/lemca_data/job";
    std::string s2 = "mkdir -p "+ dir + ";";
    INFO(s2);
    if(system( s2.c_str() )){
        std::cerr << "*** can not execute : " << s2 << std::endl;;
    };
    
    
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%Y_%m_%d_%H_%M_%S", &tstruct);
    
    m_begin = buf;
    
    dir = DirectoryManager::instance().getHome()+"/lemca_data/job/"+m_begin;
    s2 = "mkdir -p "+ dir + ";";
    INFO(s2);
    if(system( s2.c_str() )){
        std::cerr << "*** can not execute : " << s2 << std::endl;;
    };
    
    m_file = dir = DirectoryManager::instance().getHome()+"/lemca_data/job/"+m_begin+"/job.txt";
    handle60s();
}

int m_s = 0;
void JobManager::handleSeconde(){
    m_s++;
    if(m_s > 60){
        handle60s();
        m_s = 0;
    }
}

void JobManager::handle60s(){
    std::ofstream file;
    file.open(m_file);
    
    file << "PROG,BINEUSE" << std::endl;
    file << "BEGIN," << m_begin << std::endl;
    
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%Y_%m_%d_%H_%M_%S", &tstruct);
    file << "END," << buf << std::endl;
    
    file << "SESSION_H," << FrameworkVision::instance().m_qt_compteur_heure.m_heure_session << std::endl;
    file << "SESSION_HA," << FrameworkVision::instance().m_qt_compteur_heure.m_ha_session << std::endl;
    file << "SESSION_TOTAL_H," << FrameworkVision::instance().m_qt_compteur_heure.m_heure_total << std::endl;
    file << "SESSION_TOTAL_HA," << FrameworkVision::instance().m_qt_compteur_heure.m_ha_total << std::endl;
    
    file.close();
}

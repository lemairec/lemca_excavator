#include "job_manager.hpp"
#include "../util/directory_manager.hpp"
#include "../util/log.hpp"
#include "../framework.hpp"

#include <QDir>

JobManager::JobManager(){
    
}

std::string JobManager::jobDir(){
    return DirectoryManager::instance().getHome()+"/lemca_data/job";
}

std::vector<std::string> JobManager::listJobs(){
    std::vector<std::string> res;
    QDir dir(QString::fromStdString(jobDir()));
    //noms YYYY_MM_DD_HH_MM_SS -> tri alphabetique inverse = plus recent d'abord
    for(const QString & s : dir.entryList(QDir::Dirs|QDir::NoDotAndDotDot, QDir::Name|QDir::Reversed)){
        res.push_back(s.toStdString());
    }
    return res;
}

int JobManager::countPoints(const std::string & name){
    std::ifstream file(jobDir()+"/"+name+"/soil.txt");
    int n = 0;
    std::string line;
    while(std::getline(file, line)){
        if(line.size() > 5){   //ignore les lignes vides / fins de fichier
            n++;
        }
    }
    return n;
}

bool JobManager::openJob(const std::string & name){
    std::string dir = jobDir()+"/"+name;
    if(name.empty() || !QDir(QString::fromStdString(dir)).exists()){
        WARN("openJob : job introuvable " << dir);
        return false;
    }
    if(m_log_file.is_open()){ m_log_file.close(); }
    if(m_data_file.is_open()){ m_data_file.close(); }
    
    m_begin = name;   //non vide -> init() ne creera plus de nouveau job
    m_file = dir+"/job.txt";
    m_log_path = dir+"/log.txt";
    m_log_file.open(m_log_path, std::ios::app);
    m_data_path = dir+"/soil.txt";
    m_data_file.open(m_data_path, std::ios::app);
    handle60s();
    
    Framework::instance().loadMesures(m_data_path);
    INFO("openJob " << m_data_path);
    return true;
}

void JobManager::init(){
    if(m_begin.empty()){
        std::string dir = DirectoryManager::instance().getHome()+"/lemca_data/job";
#ifdef WIN32
        INFO("mkpath " << dir);
        if(!QDir().mkpath(QString::fromStdString(dir))){
            std::cerr << "*** can not create dir : " << dir << std::endl;
        }
#else
        std::string s2 = "mkdir -p "+ dir + ";";
        INFO(s2);
        if(system( s2.c_str() )){
            std::cerr << "*** can not execute : " << s2 << std::endl;;
        };
#endif
        
        
        time_t     now = time(0);
        struct tm  tstruct;
        char       buf[80];
        tstruct = *localtime(&now);
        // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
        // for more information about date/time format
        strftime(buf, sizeof(buf), "%Y_%m_%d_%H_%M_%S", &tstruct);
        
        m_begin = buf;
        
        dir = DirectoryManager::instance().getHome()+"/lemca_data/job/"+m_begin;
#ifdef WIN32
        INFO("mkpath " << dir);
        if(!QDir().mkpath(QString::fromStdString(dir))){
            std::cerr << "*** can not create dir : " << dir << std::endl;
        }
#else
        s2 = "mkdir -p "+ dir + ";";
        INFO(s2);
        if(system( s2.c_str() )){
            std::cerr << "*** can not execute : " << s2 << std::endl;;
        };
#endif
        
        m_file = DirectoryManager::instance().getHome()+"/lemca_data/job/"+m_begin+"/job.txt";
        handle60s();
        
        m_log_path = DirectoryManager::instance().getHome()+"/lemca_data/job/"+m_begin+"/log.txt";
        m_log_file.open(m_log_path);
        
        m_data_path = DirectoryManager::instance().getHome()+"/lemca_data/job/"+m_begin+"/soil.txt";
        m_data_file.open(m_data_path);
    }
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
    
    file << "PROG;EXCAVATOR" << std::endl;
    file << "BEGIN;" << m_begin << std::endl;
    
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%Y_%m_%d_%H_%M_%S", &tstruct);
    file << "END;" << buf << std::endl;
    
    /*file << "SESSION_H," << FrameworkVision::instance().m_qt_compteur_heure.m_heure_session << std::endl;
    file << "SESSION_HA," << FrameworkVision::instance().m_qt_compteur_heure.m_ha_session << std::endl;
    file << "SESSION_TOTAL_H," << FrameworkVision::instance().m_qt_compteur_heure.m_heure_total << std::endl;
    file << "SESSION_TOTAL_HA," << FrameworkVision::instance().m_qt_compteur_heure.m_ha_total << std::endl;*/
    
    file.close();
}

void JobManager::logJob(const std::string & s){
    m_log_file << s << "\n";
    m_log_file.flush();
}

void JobManager::addData(const std::string & s){
    INFO(s);
    m_data_file << s << "\n";
    m_data_file.flush();
}

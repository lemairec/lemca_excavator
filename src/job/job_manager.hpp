#ifndef JOB_MANAGER_H
#define JOB_MANAGER_H

#include "../util/include.hpp"

class JobManager  {
    std::string m_begin;
    std::string m_file;
    
    std::string jobDir();               //.../lemca_data/job
    
public:
    JobManager();

    void init();
    
    //reprise d'un job existant : on reecrit dans ses fichiers (append) et on
    //recharge ses points pH sur la carte.
    std::vector<std::string> listJobs();     //noms de dossiers, plus recent d'abord
    bool openJob(const std::string & name);
    int countPoints(const std::string & name);  //nbr de lignes du soil.txt d'un job
    const std::string & getName(){return m_begin;};
    void handleSeconde();
    void handle60s();
    
    std::string m_log_path;
    std::ofstream m_log_file;
    
    std::string m_data_path;
    std::ofstream m_data_file;
    
    void logJob(const std::string & string);
    void addData(const std::string & string);
    void rewriteData(const std::vector<std::string> & lines);  //reecrit soil.txt (edition/suppression)
};

#endif

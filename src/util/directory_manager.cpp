#include "directory_manager.hpp"
#include <iostream>
#include <fstream>
#include <string>

#include <QDir>

#include <environnement.hpp>
#include "util.hpp"
using namespace std;

DirectoryManager & DirectoryManager::instance(){
    static DirectoryManager gf;
    return gf;
}

void DirectoryManager::readFile(){
    string line;
    ifstream myfile (m_file_path);
    int i =0;
    std::cout << "readFile" << std::endl;
    if (myfile.is_open())
    {
      while ( getline (myfile,line) )
      {
          if(i==0){
              m_data_dir = line;
          }
          if(i==1){
              m_source_dir = line;
          }
          ++i;
      }
      myfile.close();
    }
}

void DirectoryManager::init(){
    std::string dir = m_home+"/lemca_data/lemca_excavator";
#ifdef WIN32
    if(!QDir().mkpath(QString::fromStdString(dir))){
        std::cerr << "*** can not create dir : " << dir << std::endl;
    }
#else
    std::string s2 = "mkdir -p "+ dir + ";";
    std::cout << s2 << std::endl;;
    if(system( s2.c_str() )){
        std::cerr << "*** can not execute : " << s2 << std::endl;;
    };
#endif
}

DirectoryManager::DirectoryManager(){
#ifdef WIN32
    m_home = QDir::homePath().toStdString();
#else
    m_home = std::getenv("HOME");
#endif
    m_file_path = m_home + "/lemca_data/lemca_excavator.txt";
    init();
    readFile();
    if(m_source_dir.empty()){
        ofstream myfile(m_file_path);
        if (myfile.is_open())
        {
            myfile << m_home+"/lemca_data/lemca_excavator";
            myfile << std::endl;
            myfile << m_home+"/lemca_excavator";
            myfile.close();
            std::cout << "write" << std::endl;
        } else {
            m_data_dir = m_home+"/lemca_data/lemca_excavator";
            m_source_dir = ProjectSourceDir2;
            std::cout << "not open :(" << std::endl;
            
        }
        readFile();
    }
    m_log_file = m_data_dir+"/log.txt";
    m_config_file = m_data_dir+"/lemca_excavator.ini";
    std::cout << "m_source_dir " << m_source_dir << std::endl;
    std::cout << "m_data_dir " << m_data_dir << std::endl;
    std::cout << "m_log_file " << m_log_file << std::endl;
    std::cout << "m_config_file " << m_config_file << std::endl;
}

const std::string & DirectoryManager::getDataDirectory(){
    return m_data_dir;
}

const std::string & DirectoryManager::getSourceDirectory(){
    return m_source_dir;
}

void DirectoryManager::makeRelativeDir(std::string dir2){
    std::string dir = DirectoryManager::instance().getDataDirectory() + dir2;
#ifdef WIN32
    INFO("mkpath " << dir);
    if(!QDir().mkpath(QString::fromStdString(dir))){
        WARN("can not create dir : " << dir);
    }
#else
    std::string s2 = "mkdir -p "+ dir + ";";
    INFO(s2);
    if(system( s2.c_str() )){
        WARN("can not execute : " << s2);
    };
#endif
}

void DirectoryManager::removeRelativeDir(std::string dir2){
    std::string dir = DirectoryManager::instance().getDataDirectory() + dir2;
#ifdef WIN32
    INFO("remove " << dir);
    if(!QDir(QString::fromStdString(dir)).removeRecursively()){
        WARN("can not remove dir : " << dir);
    }
#else
    std::string s2 = "rm -rf "+ dir + ";";
    INFO(s2);
    if(system( s2.c_str() )){
        WARN("can not execute : " << s2);
    };
#endif
}

void DirectoryManager::clearAll(){
    std::string dir = m_home+"/lemca_excavator";
#ifdef WIN32
    INFO("remove " << dir);
    if(!QDir(QString::fromStdString(dir)).removeRecursively()){
        WARN("can not remove dir : " << dir);
    }
#else
    std::string s2 = "rm -rf "+ dir + ";";
    INFO(s2);
    if(system( s2.c_str() )){
        WARN("can not execute : " << s2);
    };
#endif
}

DirectoryManager::~DirectoryManager(){
    
}

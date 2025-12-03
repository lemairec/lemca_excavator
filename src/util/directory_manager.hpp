#ifndef DIRECTORY_MANAGER_H
#define DIRECTORY_MANAGER_H

#include <string>

class DirectoryManager {
    DirectoryManager();
    void readFile();
    
    void init();
    
    std::string m_home;
    std::string m_file_path;
    std::string m_data_dir;
    std::string m_source_dir;
public:
    static DirectoryManager & instance();
    ~DirectoryManager();
    
    std::string m_log_file;
    std::string m_config_file;
    
    const std::string & getHome(){return m_home;};
    const std::string & getDataDirectory();
    const std::string & getSourceDirectory();
    
    void initDataFile();
    void initGPSFile();
    void logData(const std::string & data);
    void saveGPS(const std::string & data);
    
    void makeRelativeDir(std::string dir2);
    void removeRelativeDir(std::string dir2);
    void clearAll();
};

#endif //UTIL_H

#ifndef CSV_H
#define CSV_H

#include "include.hpp"

class CSVLine {
public :
    std::vector<std::string> m_words;
    
    bool empty();
    
    double getDouble(size_t i);
    
    void addWord(const std::string & s);
};

class CSVFile {
public :
    std::vector<CSVLine> m_lines;
    
    void importFile(const std::string & path);
    void exportFile(const std::string & path);
};

#endif

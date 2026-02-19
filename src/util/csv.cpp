#include "csv.hpp"
#include "../util/util.hpp"


bool CSVLine::empty(){
    return m_words.empty();
}


double CSVLine::getDouble(size_t i){
    std::string s1 = m_words[i];
    std::replace(s1.begin(), s1.end(), ',', '.');
    double out = std::stod(s1);
    return out;
}

void CSVLine::addWord(const std::string & s){
    m_words.push_back(s);
}

void CSVFile::importFile(const std::string & path){
    std::ifstream file(path);
    std::string line;
    
    int count_n = 0;
    int count_r = 0;
    {
        std::ifstream tmp(path);
        char c;
        while (tmp.get(c)) {
            if (c == '\n'){
                count_n++;
            }
            if (c == '\r') {
                count_r++;
            }
        }
    }
    INFO("n " << count_n << " r " << count_r);
    
    char sep = '\r';
    if(count_n > count_r){
        sep = '\n';
    }
    
    
    while (std::getline(file, line, sep)) {
        INFO("la " << line);
        std::stringstream ss(line);
        std::string cell;

        CSVLine line;
        while (std::getline(ss, cell, ';')) {
            line.m_words.push_back(cell);
        }
        INFO("word " << line.m_words.size());
        
        if(!line.empty()){
            INFO("add");
            m_lines.push_back(line);
        }
    }
}


void CSVFile::exportFile(const std::string & path){
    std::ofstream file(path);
    
    for(auto line : m_lines){
        bool first = 0;
        for(auto word : line.m_words){
            file << word;
            if(first){
                first = 1;
            } else {
                file << ";";
            }
        }
            
        file << "\n";
    }
}

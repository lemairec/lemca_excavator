#include "csv.hpp"



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
    
    char sep = '\n';
    {
        std::ifstream tmp(path);
        char c;
        while (tmp.get(c)) {
            if (c == '\n') break;
            if (c == '\r') { sep = '\r'; break; }
        }
    }
    
    while (std::getline(file, line, sep)) {
        if (!line.empty() && (line.back() == '\n' || line.back() == '\r')){
            line.pop_back();
        }
        
        std::stringstream ss(line);
        std::string cell;

        CSVLine line;
        while (std::getline(ss, cell, ';')) {
            line.m_words.push_back(cell);
        }
        
        if(line.empty()){
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

#include "csv.hpp"
#include "../util/util.hpp"


bool CSVLine::empty(){
    return m_words.empty();
}


//Les fichiers ecrits sous locale FR (le tracteur) ont les decimales a la virgule.
//On les ramene au point, puis on lit avec la locale "C" : std::stod suit la locale
//courante, donc sous fr_BE il s'arrete au '.' et "50.6552792" devient 50.
double toDouble(const std::string & s){
    std::string s1 = s;
    std::replace(s1.begin(), s1.end(), ',', '.');

    std::istringstream iss(s1);
    iss.imbue(std::locale::classic());
    double out = 0;
    iss >> out;
    if(iss.fail()){
        throw std::invalid_argument("toDouble : "+s);
    }
    return out;
}

double CSVLine::getDouble(size_t i){
    return toDouble(m_words[i]);
}

void CSVLine::addWord(const std::string & s){
    m_words.push_back(s);
}

static void split(const std::string & raw, char sep, std::vector<std::string> & out){
    out.clear();
    std::stringstream ss(raw);
    std::string cell;
    while(std::getline(ss, cell, sep)){
        out.push_back(cell);
    }
}

bool splitSoilLine(const std::string & raw, std::vector<std::string> & out){
    split(raw, ';', out);
    if(out.size() >= 6){
        return true;
    }
    //ancien format : virgule. 9 champs pile = decimales en '.', donc non ambigu.
    split(raw, ',', out);
    return out.size() == 9 || out.size() == 10;
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
    
    //CRLF -> count_n == count_r : il faut couper sur '\n' et jeter le '\r' restant,
    //sinon chaque ligne garde un '\n' en tete et la derniere ligne est perdue.
    char sep = count_n > 0 ? '\n' : '\r';
    
    
    while (std::getline(file, line, sep)) {
        if(!line.empty() && line.back() == '\r'){
            line.pop_back();
        }
        std::string raw = line;
        std::stringstream ss(line);
        std::string cell;

        CSVLine line;
        while (std::getline(ss, cell, ';')) {
            line.m_words.push_back(cell);
        }
        line.m_string = raw;
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

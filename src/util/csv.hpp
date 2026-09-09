#ifndef MY_CSV_H
#define MY_CSV_H

#include "include.hpp"

double toDouble(const std::string & s);   //accepte '.' et ',' comme separateur decimal

class CSVLine {
public :
    std::vector<std::string> m_words;
    std::string m_string;
    
    bool empty();
    
    double getDouble(size_t i);
    
    void addWord(const std::string & s);
};

//soil.txt : ';' depuis 2025-12-15 (9 puis 10 colonnes), ',' avant (9 colonnes).
//Le format virgule n'est lisible que si les decimales sont en '.' : sinon le nombre
//de champs explose et la ligne est rejetee, ce qui vaut mieux que des coordonnees
//fausses. false = ligne inexploitable.
bool splitSoilLine(const std::string & raw, std::vector<std::string> & out);

class CSVFile {
public :
    std::vector<CSVLine> m_lines;
    
    void importFile(const std::string & path);
    void exportFile(const std::string & path);
};

#endif

#ifndef LANGAGE_H
#define LANGAGE_H


#include <string>
#include <unordered_map>
#include <map>
#include <vector>

enum MyLangage {
    MyLangage_fr = 0,
    MyLangage_en = 1,
    MyLangage_de = 2,
};

class Langage  {
    std::unordered_map<std::string,std::string> m_fr;
    std::unordered_map<std::string,std::string> m_en;
    std::unordered_map<std::string,std::string> m_de;
    std::vector<std::string> m_all;
    
    Langage();
public:
    static Langage & instance();
    MyLangage m_l = MyLangage_fr;
    
    
    static const std::string & getKey(const std::string & ref);
    static void setLangage(int i);
    static void setLangage(const std::string & s);
    
    void add(std::string key, std::string trad, std::string langage);
    void verifyAll();
};
#endif


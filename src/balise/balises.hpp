#ifndef BALISES_H
#define BALISES_H

#include "../util/util.hpp"
#include "../gps/nmea_parser.hpp"
//#include "parcelle.hpp"

struct Balise : public GpsPoint {
    std::string m_color = "";
    std::string m_name = "";
    std::string m_datetime = "";
    
    double m_distance;
    
    bool operator() (int i,int j) { return (i<j);}
};

class Balises {
    //int m_page = 0;
public:
    std::vector<Balise *> m_balises;
    std::vector<Balise *> m_balises_sort;
    
    void clear();
    
    void save();
    void load();
    void addBalise(Balise * b);
    
    void sort(GpsPoint_ptr p);
};

#endif // LINE_AB_H

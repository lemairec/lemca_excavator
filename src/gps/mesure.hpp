#ifndef MESURE_H
#define MESURE_H

#include <string>

#include "nmea_parser.hpp"

class Mesure {
public :
    GpsPoint m_point;
    
    double m_ph;
    std::string m_line;   //ligne soil.txt d'origine (10 colonnes), pour reecriture sans perte
};

#endif //MESURE_H

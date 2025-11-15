#ifndef HEXA_PARSER_H
#define HEXA_PARSER_H

#include "../util/parser.hpp"
#include <cmath>


class HexaParser : public Parser {
public:
    HexaParser();
protected:
    int m_error_read = 0;
    void parseBuffer();
    bool checkBuffer();

public:
    void parseSoil();
    
    void handle100ms();
};

#endif //GPS_H

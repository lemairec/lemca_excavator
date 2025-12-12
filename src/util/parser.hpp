#ifndef PARSER_H
#define PARSER_H

#include "include.hpp"

#define BUFFER_SIZE 300

class Parser {
public:
    Parser();
    void init();
    
    void printBuffer();
    
    void readFrame(const std::string & frame);
    void readChar(char c);

    void resetBuffer();
    
    int m_nbr_error = 0;
    bool m_has_error = false;
    void error();
    
    int m_nb_error_check = 0;
    bool checkBuffer();
protected:
    size_t m_bufferIndLast = 0;
    char m_buffer[BUFFER_SIZE];
    size_t m_tempInd = 0;
    
    void debug();
    
    virtual void parseBuffer() = 0;
    
    void readUntilCommat();
    char readNextCharAndCommat();
    int getOneInt();
    int getIntWithChar(char c);
    int getHexaIntWithChar(char c);
    int readNegInt();
    int readInt();
    int readIntHexa();
    double readDouble();
    double readNegDouble();
    double readDeg();
    
    double getTimeHour(double d);
};

#endif //GPS_H

#include "parser.hpp"
#include "../util/log.hpp"
#include "../util/util.hpp"
#include "environnement.hpp"


#include <math.h>

Parser::Parser(){
    init();
    //setRef(LAT_REF, LON_REF);
}

void Parser::init(){
    resetBuffer();
}

void Parser::readFrame(const std::string & frame){
    for(char c : frame){
        readChar(c);
    }
    readChar('\n');
}

void Parser::readChar(char c){
    DEBUG(c);
    //std::cout << c;
    if(c == '$'){
        //INFO("readChar");
    } else if(c == '\n' || c == '\r'){
        parseBuffer();
        resetBuffer();
    } else {
        if(m_bufferIndLast > BUFFER_SIZE){
            resetBuffer();
        }
        this->m_buffer[m_bufferIndLast] = c;
        m_bufferIndLast++;
    }
}



bool Parser::checkBuffer(){
    int checksum = 0;
    for(size_t i =0; i < m_bufferIndLast; ++i){
        const char c = m_buffer[i];
        if (c == '*'){
            if(i < m_bufferIndLast+2){
                char c1 = m_buffer[i+1];
                char c2 = m_buffer[i+2];
                
                int i = getHexIntWithChar(c1)*16+getHexIntWithChar(c2);
                if(i == checksum){
                    return true;
                }
            }
            break;
        }
        checksum = checksum ^ c;
    }
    m_nb_error_check++;
    return false;
}


void Parser::resetBuffer(){
    m_bufferIndLast = 0;
    m_tempInd = 0;
    m_has_error = false;
    //for(size_t i = 0; i < 200; ++i){
    //  m_buffer[i] = ' ';
    //}
}


void Parser::error(){
    m_nbr_error++;
    printBuffer();
    m_has_error = true;
    WARN("error");
}

void Parser::debug(){
    /*Serial.print(m_tempInd);
     Serial.print(" ");
     Serial.print(m_buffer[m_tempInd-1]);
     Serial.print(m_buffer[m_tempInd]);
     Serial.print(m_buffer[m_tempInd+1]);
     Serial.print("\n");*/
}


void Parser::printBuffer(){
    std::string s = "";
    for(size_t i =0; i < m_bufferIndLast; ++i){
        s += m_buffer[i];
    }
    INFO(s);
}

/*bool Parser::isEqual(const char * c, size_t size){
    size_t j = m_tempInd;
    for(size_t i = 0; i < size; ++i){
        if(c[i] == m_buffer[j]){
            ++j;
        } else {
            //  INFO(c[i] << " " << m_buffer[j] << " " << i);
            return false;
        }
    }
    return true;
}*/

void Parser::readUntilCommat(){
    while(m_tempInd < m_bufferIndLast){
        if(m_buffer[m_tempInd] == ','){
            ++m_tempInd;
            return;
        }
        ++m_tempInd;
    }
    error();
}

char Parser::readNextCharAndCommat(){
    if(m_tempInd+2 < m_bufferIndLast){
        char c = m_buffer[m_tempInd];
        m_tempInd++;
        char c2 = m_buffer[m_tempInd];
        m_tempInd++;
        if(c2 == ','){
            return c;
        } else {
            WARN("'" << c2 << "'");
        }
    }
    error();
    return '*';
}


int Parser::getOneInt(){
    char c = m_buffer[m_tempInd];
    m_tempInd++;
    return getIntWithChar(c);
}

int Parser::getIntWithChar(char c){
    if(c =='0'){
        return 0;
    } else if(c =='1'){
        return 1;
    } else if(c =='2'){
        return 2;
    } else if(c =='3'){
        return 3;
    } else if(c =='4'){
        return 4;
    } else if(c =='5'){
        return 5;
    } else if(c =='6'){
        return 6;
    } else if(c =='7'){
        return 7;
    } else if(c =='8'){
        return 8;
    } else if(c =='9'){
        return 9;
    } else {
        error();
        INFO("'" << c << "'" << (int)c);
        return 0;
    }
}

int Parser::getHexaIntWithChar(char c){
    if(c == '0'){
        return 0x0;
    } else if(c == '1'){
        return 0x1;
    } else if(c == '2'){
        return 0x2;
    } else if(c == '3'){
        return 0x3;
    } else if(c == '4'){
        return 0x4;
    } else if(c == '5'){
        return 0x5;
    } else if(c == '6'){
        return 0x6;
    } else if(c == '7'){
        return 0x7;
    } else if(c == '8'){
        return 0x8;
    } else if(c == '9'){
        return 0x9;
    } else if(c == 'a'){
        return 0xA;
    } else if(c == 'A'){
        return 0xA;
    } else if(c == 'b'){
        return 0xB;
    } else if(c == 'B'){
        return 0xB;
    } else if(c == 'c'){
        return 0xC;
    } else if(c == 'C'){
        return 0xC;
    } else if(c == 'd'){
        return 0xD;
    } else if(c == 'D'){
        return 0xD;
    } else if(c == 'e'){
        return 0xE;
    } else if(c == 'E'){
        return 0xE;
    } else if(c == 'f'){
        return 0xF;
    } else if(c == 'F'){
        return 0xF;
    }
    return 0;
}


int Parser::readNegInt(){
    bool neg = false;
    if(m_buffer[m_tempInd] == '-'){
        neg = true;
        ++m_tempInd;
    }
    int res  = readInt();
    if(neg){
        return -res;
    } else {
        return res;
    }
}

int Parser::readIntHexa(){
    int res = 0;
    
    while(m_tempInd < m_bufferIndLast){
        char c = m_buffer[m_tempInd];
        if(c == ',' || c == ';'){
            ++m_tempInd;
            return res;
        } else {
            res = res*16 + getHexaIntWithChar(c);
            ++m_tempInd;
        }
    }
    return res;
}

int Parser::readInt(){
    int res = 0;
    while(m_tempInd < m_bufferIndLast){
        char c = m_buffer[m_tempInd];
        if(c == ',' || c == ';'){
            ++m_tempInd;
            return res;
        } else {
            res = res*10 + getIntWithChar(c);
            ++m_tempInd;
        }
    }
    return res;
}

double Parser::readDouble(){
    double res = 0;
    double virgule_part = 1;
    bool virgule = false;
    while(m_tempInd < m_bufferIndLast){
        char c = m_buffer[m_tempInd];
        int number = 0;
        if(c == ','|| c == ';'){
            ++m_tempInd;
            return res;
        } else if(c =='.'){
            virgule = true;
        } else {
            number = getIntWithChar(c);
        }
        if(!virgule){
            res = res * 10 + number;
        } else {
            res = res + number * virgule_part;
            virgule_part = virgule_part * 0.1;
        }
        ++m_tempInd;
    }
    error();
    return 0.0;
}

double Parser::readNegDouble(){
    bool neg = false;
    if(m_buffer[m_tempInd] == '-'){
        neg = true;
        ++m_tempInd;
    }
    double res = readDouble();
    if(neg){
        return -res;
    } else {
        return res;
    }
}

double Parser::readDeg()
{
    double d = readDouble();
    int h = d/100;
    double minu = (d-(h*100));
    return h + minu/60.0;
}


double Parser::getTimeHour(double d)
{
    int h = d/10000;
    int minu = (d/100-(h*100));
    double s = d - h*10000.0 - minu*100.0;
    double res = h + minu/60.0 + s/3600.0;
    //INFO(d << " " << h << ":" << minu << ":" << s << " " << res);
    return res;
}

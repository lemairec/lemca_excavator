#include "hexa_parser.hpp"
#include "../util/util.hpp"
#include "../util/include.hpp"
#include "../framework.hpp"
//#include "gps_module.hpp"

HexaParser::HexaParser(){
}
void HexaParser::parseBuffer(){
    if(m_bufferIndLast > 0){
        std::stringstream ss;
        for(size_t i =0; i < m_bufferIndLast; ++i){
            ss << " " << std::setw(2) << std::setfill('0') << std::hex << (int)((uchar)m_buffer[i]);
        }
        std::string s2 = "parse => " + ss.str();
        
        //Framework & f = Framework::Instance();
        //f.addSerialMessage(s2);
        if(m_buffer[0] == 0x01 && m_buffer[1] == 0x03 && m_buffer[2] == 0x0e){
            parseSoil();
        }
        this->resetBuffer();
    }
}

bool HexaParser::checkBuffer(){
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
    m_error_read++;
    return false;
}

int m_parser_i = 0;
void HexaParser::handle100ms(){
    parseBuffer();
    m_parser_i++;
    if(m_parser_i>9){
        m_parser_i = 0;
        Framework & f = Framework::instance();
        std::vector<unsigned char> l = {0x01, 0x03, 0x00, 0x00, 0x00, 0x07, 0x04, 0x08};
        f.m_serialModule.writePort3SoilHexa(l);
    }
    
}

void HexaParser::parseSoil(){
    int humidity = ((uchar)m_buffer[3])*256+((uchar)m_buffer[4]);
    int temperature = ((uchar)m_buffer[5])*256+((uchar)m_buffer[6]);
    int conductivity = ((uchar)m_buffer[7])*256+((uchar)m_buffer[8]);
    int ph = ((uchar)m_buffer[9])*256+((uchar)m_buffer[10]);
    int n = ((uchar)m_buffer[11])*256+((uchar)m_buffer[12]);
    int p = ((uchar)m_buffer[13])*256+((uchar)m_buffer[14]);
    int k = ((uchar)m_buffer[15])*256+((uchar)m_buffer[16]);
    
    double humidity_ = humidity*0.1;
    double temperature_ = temperature*0.1;
    double conductivity_ = conductivity*1;
    double ph_ = ph*0.1;
    double n_ = n*1;
    double p_ = p*1;
    double k_ = k*1;
    
    //Framework & f = Framework::Instance();
    std::string s = strprintf("res => hum %.1f  temp %.1f  cond %.1f ph %.1f  n %.1f  p %.1f  k %.1f", humidity_, temperature_, conductivity_ , ph_, n_, p_, k_);
    //f.addSerialMessage(s);
    
}

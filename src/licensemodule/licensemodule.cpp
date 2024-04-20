#include "licensemodule.hpp"
#include "../util/util.hpp"

#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <string>

#define NUMBER_CHAR 12
#define MODULO 30

/*std::string exec(const std::string&  command) {
    system((command + " > temp.txt").c_str());
    
    std::ifstream ifs("temp.txt");
    std::string ret{ std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>() };
    ifs.close(); // must close the inout stream so the file can be cleaned up
    if (std::remove("temp.txt") != 0) {
        perror("Error deleting temporary file");
    }
    INFO(ret);
    return ret;
}


char inttoChar(int i){
    if(i < 9){
        return i+'0';
    } else {
        return i-9+ 'A';
    }
}

int chartoInt(char c){
    if(c >= '0' && c <= '9'){
        return c-'0';
    }
    if(c >= 'A' && c <= 'Z'){
        return c-'A'+9;
    }
    if(c >= 'a' && c <= 'z'){
        return c-'a'+9;
    }
    return 0;
}

std::string getMyMachineHash(){
    int res[12];
    for(int i = 0; i< 12; ++i){
        res[i] = 0;
    }
    
#ifdef UNIX
    INFO("UNIX");
    int i = 0;
    std::string str = exec("cat /proc/cpuinfo");
    //INFO(str);
    if(str.size() == 0){
        return "error-0000";
    }
    for(char& c : str) {
        int j = i%NUMBER_CHAR;
        res[j] = (res[j]+chartoInt(c))%35;
        i++;
    }

    str = exec("cat /sys/class/net/eth0/address");
    if(str.size() == 0){
        return "error-0001";
    }
    for(char& c : str) {
        int j = i%NUMBER_CHAR;
        res[j] = (res[j]+chartoInt(c))%35;
        //INFO(j << " " << res[j] << " " << c << " " << chartoInt(c));
        i++;
    }
#else
    return "error-0002";
#endif
    
    
    std::ostringstream oss;
    for(int i = 0; i< NUMBER_CHAR; ++i){
        //INFO(res[i]);
        char c = inttoChar(res[i]);
        oss << c;
    }
    std::string s_res = oss.str();
    
    return s_res;
}


std::string getLicenseForHash(std::string s){
    std::string code = "HGZ8HDYH6GD5";
    std::ostringstream oss;
    
    uint64_t temp = 0;
    for(char& c : s) {
        temp = temp*30+chartoInt(c);
    }
    if(s.size() == 12 && code.size() == 12){
        for(int i = 0; i< 12; ++i){
            int r  = temp%MODULO;
            if(r<0){
                r+=MODULO;
            }
            temp = temp/MODULO;
            
            int res = r + chartoInt(code[i]);
            char res2 = inttoChar(res%35);
            oss << res2;
        }
    } else {
        return "kooijdviosivjssfvs";
    }
    return oss.str();
}*/


LicenseModule::LicenseModule()
{
    //m_machineHash = getMyMachineHash();
    //INFO(m_machineHash);
}

LicenseModule::~LicenseModule(void)
{
}

std::string zz_dech2(std::string mess, std::string clef) {
    std::string dict = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    int dict_len = dict.size();
    std::string res = "";
    //INFO("\n\nzzdech2 "<< dict.size() << "\n");
    size_t rlLong = mess.size();
    size_t rlCrypt = clef.size();
    int rlVar0 = 0;
    if(rlLong>0 && rlCrypt>0){
        size_t rlPos=0;
        for(size_t i=0; i<rlLong; i++){
            if(mess[i] == '-'){
                res+="-";
                continue;
            }
            int rlVar1=dict.find(mess[i]);
            int rlVar2=dict.find(clef[rlPos]);
            int rlVarP=(rlVar1-rlVar2-rlVar0)%dict_len;
            //INFO(i<< ", " << rlVar0 << " " << rlVar1 << " " << rlVar2 << " " << rlVarP);
            rlVar0 = (rlVar0+rlVar1)%dict_len;
            if(rlVarP<0){
                rlVarP=rlVarP+dict_len;
            }
            res = res+dict[rlVarP];
            rlPos=rlPos+1;
            if(rlPos>rlCrypt-1){
                rlPos=0;
            }
        }
    }
    return res;
}

void LicenseModule::load(const Config & config)
{
    std::string clef = "ATBSKUYCJHPHTYU";
    
    std::string licence = config.m_licence;
    
    if(licence.size() != 18){
        m_licence_result = LicenceFailLen;
        return;
    }
    
    if(licence.size() == 18){
        INFO("licence : " << licence);
        
        m_panel_mcu = "";
        if(config.m_licence_panel.size() <= 6){
            m_panel_mcu += config.m_licence_panel;
            for(int i = config.m_licence_panel.size(); i < 6; ++i){
                m_panel_mcu += " ";
            }
        } else {
            m_panel_mcu += config.m_licence_panel.substr(0,3);
            m_panel_mcu += config.m_licence_panel.substr(config.m_licence_panel.size()-3,3);
        }
        
        if(config.m_licence_boitier.size() <= 6){
            m_panel_mcu += config.m_licence_boitier;
            for(int i = config.m_licence_boitier.size(); i < 6; ++i){
                m_panel_mcu += " ";
            }
        } else {
            m_panel_mcu += config.m_licence_boitier.substr(0,3);
            m_panel_mcu += config.m_licence_boitier.substr(config.m_licence_boitier.size()-3,3);
        }
        
        INFO("panel boitier : '" << m_panel_mcu << "'");
        
        m_licence_decode = zz_dech2(licence, clef);
        INFO("licence decode : " << m_licence_decode);
        
        m_licence_f = m_licence_decode.substr(12,6);
        INFO(m_licence_f);
        
        INFO("licence : " << licence);
        
        /*time_t     now = time(0);
        struct tm  tstruct;
        char       buf[80];
        tstruct = *localtime(&now);
        // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
        // for more information about date/time format
        strftime(buf, sizeof(buf), "%Y_%m_%d_%H_%M", &tstruct);

        int year =tstruct.tm_year; //1900
        
        INFO(buf);*/
        
        m_demo_ok = false;
        if(m_licence_f == "LEMCA1"){
            m_licence_result = LicenceOK;
            return;
        } else {
            m_licence_result = LicenceFail;
        }
    }
}


bool LicenseModule::isOk(){
    if(m_licence_result == LicenceOK){
        return true;
    } else {
        return m_demo_ok;
    }
}

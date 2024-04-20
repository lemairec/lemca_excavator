#ifndef LICENSE_MODULE_H
#define LICENSE_MODULE_H

#include "../config/config.hpp"

enum LicenceResult {
    LicenceNone = 0,
    LicenceOK,
    LicenceFailLen,
    LicenceFailBegin,
    LicenceFail
};

class LicenseModule
{
public:
    LicenseModule();
    ~LicenseModule();
    
    LicenceResult m_licence_result = LicenceFail;
    
    std::string m_panel_mcu;
    
    
    std::string m_licence;
    std::string m_licence_tiret;
    std::string m_licence_decode;
    std::string m_licence_f;
    
    bool m_demo_ok = false;
    
    void load(const Config & config);

    /*const std::string & getMachineHash(){
        return m_machineHash;
    }*/
    
    bool isOk();
private:
    //std::string m_machineHash;
};

#endif //LicenseModule


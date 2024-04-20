#ifndef SYNCHRO_H
#define SYNCHRO_H

#include "../config/enum.hpp"
#include "../util/include.hpp"

class Synchro {
public:
   
    void beginSynchro();
    void onParcellesResponse(const std::string & s);
    void onParcelleResponse(const std::string & m_name, const std::string & s);
    
    void postBalises();
    void onBalisesResponse(const std::string & str);
    void erreurSynchro(const std::string & s, const std::string & s2);
    
    SynchroState m_synchro_state = SynchroState_None;
    int m_synchro_perc;
    std::string m_logs;
    
    void addLog(const std::string & s);
    
    void doNextStepSynchro();
};

#endif

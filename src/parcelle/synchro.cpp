#include "synchro.hpp"
#include <math.h>
#include "../util/log.hpp"
#include "../util/util.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

#include "environnement.hpp"
#include "../framework.hpp"
#include "../util/directory_manager.hpp"

#include <QDateTime>
#include "../lib/SimpleJSON/src/JSON.h"


void Synchro::beginSynchro(){
    m_synchro_state = SynchroState_Begin;
    m_synchro_perc = 0;
    Framework::instance().m_qt_network.getParcelles();
    m_logs = "";
    addLog("begin");
    
}

void Synchro::erreurSynchro(const std::string & s, const std::string & s2){
    m_synchro_state = SynchroState_Fail;
    addLog("erreur");
    addLog(s);
    addLog(s2);
}

void Synchro::addLog(const std::string & s){
    m_logs += s + "\n";
}

void Synchro::onParcellesResponse(const std::string & str){
    addLog("parcelles reponse");
    m_synchro_state = SynchroState_Continue;
    m_synchro_perc = 10;
    /*JSONValue *json = JSON::Parse(str.c_str());
    for(auto p : f.m_parcelles.m_parcelles){
        p->m_is_on_server = false;
        p->m_todo = "";
    }
    
    for(auto parcelle : json->AsArray()){
        JSONObject root = parcelle->AsObject();
        std::string name = w_to_string(root[L"name"]->AsString());
        std::string datetime = w_to_string(root[L"datetime"]->AsString());
        
        Parc * p = f.m_parcelles.findParcelle(name);
        
        
        if(p == NULL){
            INFO("not found");
            Parc * p = f.m_parcelles.addParcelle(name);
            p->m_todo = "get";
            p->m_is_on_server = true;
        } else {
            if(p->m_datetime_server != datetime){
                INFO(p->m_datetime_server << " " << datetime);
                p->m_todo = "synchro";
            }
            p->m_is_on_server = true;
        }
        //p.m_datetime = parcelle["datetime"].get_str();
        //m_parcelles.push_back(p);
    }
    
    for(auto p : f.m_parcelles.m_parcelles){
        if(!p->m_is_on_server){
            p->m_todo = "set";
        }
    }
    
    for(auto p : f.m_parcelles.m_parcelles){
        INFO(p->m_name << " " << p->m_todo << " " << p->m_is_on_server);
    }*/
    INFO("end");
    doNextStepSynchro();
}

void Synchro::doNextStepSynchro(){
    m_synchro_state = SynchroState_Continue;
    /*f.m_parcelles.save();
    int i = 0;
    int nb = f.m_parcelles.m_parcelles.size();
    for(auto p : f.m_parcelles.m_parcelles){
        INFO("parcelle :" << p->m_name << " " << p->m_todo);
        if(p->m_todo == "get"){
            std::string s = ("get " + p->m_name);
            addLog(s);
            Framework & f = Framework::instance();
            f.m_qt_network.getParcelle(p->m_name);
            m_synchro_perc = 10 + i*100/nb;
            return;
        }
        if(p->m_todo == "set" || p->m_todo == "synchro"){
            std::string s = (p->m_todo + p->m_name);
            addLog(s);
            Framework & f = Framework::instance();
            std::string  file_job = DirectoryManager::instance().getDataDirectory() + "/parcelle/" + p->m_name + ".json";
            std::ifstream file(file_job);
            std::stringstream strStream;
            strStream << file.rdbuf(); //read the file
            std::string str = strStream.str();
            f.m_qt_network.saveParcelle(p->m_name, str);
            m_synchro_perc = 10 + i*90/nb;
            return;
        }
        ++i;
    }*/
    
    m_synchro_perc = 90;
    
    addLog("parcelle end");
    postBalises();
}

void Synchro::onParcelleResponse(const std::string & name, const std::string & s){
    std::string s2 = ("response " + name);
    addLog(s2);
    
    std::string path = DirectoryManager::instance().getDataDirectory() + "/parcelle/" + name + ".json";
    std::ofstream file;
    file.open(path, std::ios::out);
    
    file << s;
    file.close();
    /*Parc * p = f.m_parcelles.findParcelle(name);
    p->m_todo = "";
    
    JSONValue *json = JSON::Parse(s.c_str());
    JSONObject root = json->AsObject();
    p->m_datetime_server = w_to_string(root[L"datetime"]->AsString());*/
    
    doNextStepSynchro();
    
}


void Synchro::postBalises(){
    Framework & f = Framework::instance();
    
    addLog("post balise");
    
    std::string  file_job = DirectoryManager::instance().getDataDirectory() + "/balises.json";
    std::ifstream file(file_job);
    std::stringstream strStream;
    strStream << file.rdbuf(); //read the file
    std::string str = strStream.str(); //str holds the content of the file
    
    f.m_qt_network.postBalises(str);
    
}

void Synchro::onBalisesResponse(const std::string & str){
    Framework & f = Framework::instance();
    std::string path = DirectoryManager::instance().getDataDirectory() + "/balises.json";
    std::ofstream file;
    file.open(path, std::ios::out);
    
    file << str;
    file.close();
    
    f.m_balises.load();
    
    INFO(str);
    addLog("balise complete");
    m_synchro_perc = 100;
    addLog("complete");
    m_synchro_state = SynchroState_Complete;
    addLog("FIN");
    
}

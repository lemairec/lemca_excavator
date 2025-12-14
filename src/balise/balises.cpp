#include "balises.hpp"
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


void Balises::clear(){
    m_balises.clear();
    save();
}



void Balises::load(){
    m_balises.clear();
    
    std::string file_parcelles = DirectoryManager::instance().getDataDirectory() + "/balises.json";
    std::ifstream file(file_parcelles);
    std::stringstream strStream;
    strStream << file.rdbuf(); //read the file
    std::string str = strStream.str(); //str holds the content of the file
    
    if(!str.empty()){
        JSONValue *json = JSON::Parse(str.c_str());
        for(auto balise : json->AsArray()){
            Balise * b = new Balise();
            JSONObject root = balise->AsObject();
            b->m_latitude = root[L"lat"]->AsNumber();
            b->m_longitude = root[L"lon"]->AsNumber();
            auto it = root.find(L"alt");
            if(it != root.end()){
                b->m_altitude = root[L"alt"]->AsNumber();
            }
            
            b->m_datetime = w_to_string(root[L"my_datetime"]->AsString());
            b->m_name = w_to_string(root[L"name"]->AsString());
            b->m_color = w_to_string(root[L"color"]->AsString());
            if(b->m_name[0] == 'i' && b->m_name[1] == 'r'){
                m_balises2.push_back(b);
                INFO(b->m_name);
            }
            m_balises.push_back(b);
        }
    }
    
    Framework & f = Framework::instance();
    for(auto b: m_balises){
        f.m_position_module.setXY(*b);
    }
    
    save();
}

void Balises::save(){
    JSONObject root;
    JSONArray array;
    for(auto b : m_balises){
        JSONObject value;
        value[L"lat"] = new JSONValue(b->m_latitude);
        value[L"lon"] = new JSONValue(b->m_longitude);
        value[L"alt"] = new JSONValue(b->m_altitude);
        value[L"my_datetime"] = new JSONValue(string_to_w(b->m_datetime));
        value[L"name"] = new JSONValue(string_to_w(b->m_name));
        value[L"color"] = new JSONValue(string_to_w(b->m_color));
        array.push_back(new JSONValue(value));
    }
    JSONValue * value = new JSONValue(array);
    std::string s = w_to_string(value->Stringify());
    std::string path = DirectoryManager::instance().getDataDirectory() + "/balises.json";
    std::ofstream file;
    file.open(path, std::ios::out);

    file << s;
    file.close();
}

bool myfunction (Balise * i,Balise * j) { return (i->m_distance<j->m_distance); }

void Balises::sort(GpsPoint_ptr p){
    m_balises_sort.clear();
    for(auto b : m_balises){
        b->m_distance = std::sqrt(b->distanceCarre(*p));
        m_balises_sort.push_back(b);
    }
    std::sort (m_balises_sort.begin(), m_balises_sort.end(), myfunction);
}

void Balises::addBalise(Balise *b){
    m_balises.push_back(b);
    save();
}

void Balises::addBalise(const std::string & name, double latitude, double longitude, double altitude){
    Framework & f = Framework::instance();
    
    Balise * b = new Balise();
    b->m_latitude = latitude;
    b->m_longitude = longitude;
    b->m_altitude = altitude;
    b->m_datetime = "";
    b->m_name =  name;
    b->m_color = "red";
    b->m_is_synchro = false;
    m_balises.push_back(b);
    std::string s = strprintf("%s,%.7f,%.7f,%.2f", b->m_name.c_str(), b->m_latitude, b->m_longitude, b->m_altitude);
    f.m_job_manager.addData(s);
    f.m_position_module.setXY(*b);
    save();
}


void Balises::newBalise(){
    Framework & f = Framework::instance();
    GpsPoint_ptr p = f.m_lastPoint;
   
    
    if(p){
        Balise * b = new Balise();
        b->m_latitude = p->m_latitude;
        b->m_longitude = p->m_longitude;
        b->m_altitude = p->m_altitude;
        b->m_datetime = p->m_time;
        b->m_name =  strprintf("b_%i", p->m_time);
        b->m_color = "red";
        b->m_is_synchro = false;
        m_balises_new.push_back(b);
        m_balises.push_back(b);
        std::string s = strprintf("%s,%.7f,%.7f,%.2f", b->m_name.c_str(), b->m_latitude, b->m_longitude, b->m_altitude);
        f.m_job_manager.addData(s);
        f.m_position_module.setXY(*b);
        save();
    }
    
}

void Balises::importFile(const std::string & path){
    std::ifstream file(path);
    std::string line;
    int count = 0;
    
    char sep = '\n';
    {
        std::ifstream tmp(path);
        char c;
        while (tmp.get(c)) {
            if (c == '\n') break;
            if (c == '\r') { sep = '\r'; break; }
        }
    }
    
    while (std::getline(file, line, sep)) {
        if (!line.empty() && (line.back() == '\n' || line.back() == '\r')){
            line.pop_back();
        }
    }
}

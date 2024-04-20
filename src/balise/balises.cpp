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

void Balises::addBalise(Balise *b){
    m_balises.push_back(b);
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
        for(auto parcelle : json->AsArray()){
            Balise * b = new Balise();
            JSONObject root = parcelle->AsObject();
            b->m_latitude = root[L"lat"]->AsNumber();
            b->m_longitude = root[L"lon"]->AsNumber();
            b->m_datetime = w_to_string(root[L"my_datetime"]->AsString());
            b->m_name = w_to_string(root[L"name"]->AsString());
            b->m_color = w_to_string(root[L"color"]->AsString());
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

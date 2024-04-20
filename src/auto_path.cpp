#include "auto_path.hpp"
#include "util/directory_manager.hpp"
#include "util/log.hpp"
#include "util/util.hpp"
#include "gps/nmea_parser.hpp"

#include "framework.hpp"

#include <QDateTime>
#include "lib/SimpleJSON/src/JSON.h"


#include <time.h>


void AutoPath::init(){
    struct tm tm;
    strptime(m_datetime.c_str(), "%Y-%m-%dT%H:%M:%S", &tm);
    
    char buf[255];
    m_datetime_fr =  strftime(buf, sizeof(buf), "%d/%m/%Y %H:%M", &tm);
    m_datetime_fr = std::string(buf);
}

AutoPathModule::AutoPathModule(){
    DirectoryManager::instance().makeRelativeDir("/autopaths");
    QDateTime date = QDateTime::currentDateTime();
    QString s = date.toString("yyyyMMddThhmm");
    std::string m_date_str = s.toUtf8().constData();
}

void AutoPathModule::initOrLoadConfig(Config & config){
    load();
}

AutoPath * AutoPathModule::findByLabel(const std::string & label){
    for(auto p : m_auto_paths){
        if(p->getLabel() == label){
            return p;
        }
    }
    return NULL;
}

std::string getLabelStr(JSONObject & root, const std::wstring & key){
    auto label = root[key];
    if(label){
        return w_to_string(label->AsString());
    } else {
        return "";
    }
}

void AutoPathModule::load(){
    std::string file_parcelles = DirectoryManager::instance().getDataDirectory() + "/autopaths/__autopath.json";
    std::ifstream file(file_parcelles);
    std::stringstream strStream;
    strStream << file.rdbuf(); //read the file
    std::string str = strStream.str(); //str holds the content of the file
    
    m_auto_paths.clear();
    if(!str.empty()){
        JSONValue *json = JSON::Parse(str.c_str());
        for(auto parcelle : json->AsArray()){
            AutoPath * p = new AutoPath();
            JSONObject root = parcelle->AsObject();
            p->m_name = getLabelStr(root, L"name");
            p->m_datetime = getLabelStr(root, L"my_datetime");
            p->m_file_name = getLabelStr(root, L"filename");
            //p.m_datetime = parcelle["datetime"].get_str();
            m_auto_paths.push_back(p);
        }
    }
    
    //INFO("toto" << m_auto_paths.size());
    save();
}

void AutoPathModule::save(){
    JSONObject root;
    JSONArray array;
    for(auto p : m_auto_paths){
        JSONObject value;
        value[L"name"] = new JSONValue(string_to_w(p->m_name));
        value[L"my_datetime"] = new JSONValue(string_to_w(p->m_datetime));
        value[L"filename"] = new JSONValue(string_to_w(p->m_file_name));
        array.push_back(new JSONValue(value));
    }
    JSONValue * value = new JSONValue(array);
    std::string s = w_to_string(value->Stringify());
    std::string path = DirectoryManager::instance().getDataDirectory() + "/autopaths/__autopath.json";
    std::ofstream file;
    file.open(path, std::ios::out);
    
    file << s;
    file.close();
}

void AutoPathModule::clear(){
    DirectoryManager::instance().removeRelativeDir("/parcelle");
    DirectoryManager::instance().makeRelativeDir("/parcelle");
    load();
}

void AutoPathModule::init_save(const std::string & name){
    Framework & f = Framework::instance();
    AutoPath * p = new AutoPath();
    p->m_datetime = f.m_datetime;
    p->m_name = name;
    
    std::string dt = "";
    for(auto c : p->m_datetime){
        if(c == '.'){
            dt += "_";
        } else if(c == ':'){
            dt += "_";
        } else {
            dt += c;
        }
    }
    std::string filename = dt+"-"+name;
    p->m_file_name = filename+".path";
    std::string path  = DirectoryManager::instance().getDataDirectory() + "/autopaths/"+ p->m_file_name;
    std::string debug_path = path + "_debug";
    
    auto list = m_auto_paths;
    
    m_auto_paths.clear();
    m_auto_paths.push_back(p);
    for(auto p2 : list){
        m_auto_paths.push_back(p2);
    }
    
    m_file_save_stream.open(path, std::ios::out);
    if(m_debug){
        m_file_save_debug_stream.open(debug_path, std::ios::out);
    }
    
    save();
    m_save = true;
    /*if(m_first){
        //loadAutoPath("/Users/lemairec/lemca_data/agrigpspi/job/test.record");
    }
    m_first = false;*/
}


void AutoPathModule::beginAutoPath(const std::string & s){
    INFO("beginAutoPath " << s);
    AutoPath * p = findByLabel(s);
    m_begin = myTimeInit();
    m_time_ms = 0;
    if(p){
        std::string path = DirectoryManager::instance().getDataDirectory() + "/autopaths/"+ p->m_file_name;
        if(path == ""){
            path = DirectoryManager::instance().getDataDirectory() + "/autopaths/"+p->m_name+".path";
        }
        m_load_progress = 0;
        m_load_nb_l = 0;
        m_load_i = 0;
        std::ifstream input(path);
        INFO(path);
        double nb = 0;
        for( std::string line; getline( input, line ); )
        {
            ++nb;
        }
        
        m_load_nb_l = nb;
        INFO("beginAutoPath " << s << " " << m_load_nb_l);
        
        m_load_path = path;
    }
}

void AutoPathModule::workAutoPath(){
    if(m_load_progress == -1){
        return;
    }
    NmeaParser parser;
    parser.m_auto_path = true;
    
    
    std::ifstream input2(m_load_path);
    size_t i = 0;
    int round = 0;
    int end = m_load_i + 1000;
    for( std::string line; getline( input2, line ); )
    {
        ++i;
        if(i>= m_load_i){
            //INFO(line);
            parser.readFrame(line);
            
            m_load_progress = i*100/m_load_nb_l;
            if(m_load_progress!= round){
                round = m_load_progress;
                INFO(round << " % -- " << m_time_ms << " ms");
            }
            m_load_i = i;
        }
        if(i> end){
            myTime time_end = myTimeInit();
            m_time_ms = myTimeDurationMs(m_begin, time_end);
            
            return;
        }
    }
    
    m_load_progress = 101;
    INFO("workAutoPath end " << m_time_ms << " ms");
    
    Framework & f = Framework::instance();
    f.setEtat(Etat_OK);
    
}

void AutoPathModule::onNewFrame(const std::string & s){
    if(m_save && m_debug){
        m_file_save_debug_stream << s << std::endl;
    }
}

void AutoPathModule::onGGAFrame(GGAFrame_ptr gga){
    if(m_save){
        m_ggas.push_back(gga);
        if(m_ggas.size()>9){
            m_ggas.pop_back();
        }
    }
}

void AutoPathModule::onRMCFrame(RMCFrame_ptr rmc){
    if(m_save){
        m_rmcs.push_back(rmc);
        if(m_rmcs.size()>9){
            bool ignore = true;
            for(auto r : m_rmcs){
                if(r->m_vitesse_kmh > 0.4){
                    ignore = false;
                }
            }
            if(!ignore){
                Framework & f = Framework::instance();
                GGAFrame_ptr gga = f.m_position_module.m_last_gga;
                
                double altitude = 0;
                for(auto g : m_ggas){
                    altitude += g->m_altitude;
                }
                altitude=altitude/m_ggas.size();
                
                if(gga == NULL){
                    return;
                }
                
                char c[100];
                int time = std::round(f.m_position_module.m_last_gga->m_time*10);
                int alt = std::round(altitude*100);
                snprintf(c, 100, "%d.%d,%d,%d.%d", time/10, time%10, gga->m_fix, alt/100, alt%100);
                m_file_save_stream << c;
                
                
                for(auto r : m_rmcs){
                    int lat = std::round(r->m_latitude*10000000);
                    int lon = std::round(r->m_longitude*10000000);
                    //INFO(r->m_latitude << " "<< r->m_longitude);
                    //INFO(lat << " "<< lon);
                    snprintf(c, 100, ",%08X,%08X", lat, lon);
                    m_file_save_stream << c;
                }
                m_file_save_stream << c;
                m_file_save_stream << ",*\n";
            }
            m_rmcs.clear();
            m_ggas.clear();
        }
    }
}

void AutoPathModule::onNewImportantPoint(GpsPoint_ptr p){
    double d_min = 0;
    m_i_min = 0;
    for(size_t i = 0; i < m_paths.size(); ++i){
        double d = m_paths[i]->distance(*p);
        if(d_min == 0 || d < d_min){
            d_min = d;
            m_i_min = i;
        }
    }
}

void AutoPathModule::onEvent100ms(){
    if(m_load_progress < 101){
        workAutoPath();
        return;
    }
}

void AutoPathModule::onNewPoint(GpsPoint_ptr p, double deplacement_x, double deplacement_y){
    if(m_load_progress < 101){
        return;
    }
    
    double d_min = 0;
    
    size_t end = m_i_min+10;
    if(end >= m_paths.size()){
        end = m_paths.size();
    }
    for(size_t i = m_i_min; i < end; ++i){
        double d = m_paths[i]->distance(*p);
        if(d_min == 0 || d < d_min){
            d_min = d;
            m_i_min = i;
        }
    }
    if(m_i_min>2){
        m_i_min_before = m_i_min-2;
    } else {
        m_i_min_before = 0;
    }
    //INFO(m_i_min_before);
    m_i_min_after = m_i_min+2;
    
    if(m_i_min_after < m_paths.size()){
        m_pt_before = m_paths[m_i_min_before];
        m_pt_after = m_paths[m_i_min_after];
        
        double x = p->m_x;
        double y = p->m_y;
        
        double x_a = m_pt_before->m_x;
        double y_a = m_pt_before->m_y;
        double x_b = m_pt_after->m_x;
        double y_b = m_pt_after->m_y;

        double x_v = x_b-x_a;
        double y_v = y_b-y_a;
        //double m_proj_x_segment = x_v;
        //double m_proj_y_segment = y_v;
        double d_v = sqrt(x_v*x_v + y_v*y_v);
        x_v = x_v/d_v;
        y_v = y_v/d_v;
        
        double bh = (x-x_a)*x_v+(y-y_a)*y_v;
        double m_proj_x = x_a + bh*x_v;
        double m_proj_y = y_a + bh*y_v;
        
        
        double ah = sqrt((m_proj_x-x)*(m_proj_x-x) + (m_proj_y-y)*(m_proj_y-y));
        double m_proj_distance = ah;
        
        //double m_proj_prod_vect = deplacement_x*x_v+deplacement_y*y_v;
        
        double det = (m_proj_x-x)*deplacement_y-(m_proj_y-y)*deplacement_x;
        if(det < 0){
            m_proj_distance = -m_proj_distance;
        }
        m_distance = m_proj_distance;
        /*if(m_proj_prod_vect > 0){
            m_sens_AB = true;
        } else {
            m_sens_AB = false;
        }*/
        double alt = (m_pt_before->m_altitude+m_pt_after->m_altitude)/2;
        m_diff_h = p->m_altitude - alt;
    } else {
        m_pt_before = NULL;
        m_pt_after = NULL;
        m_distance = 0.0;
        m_diff_h = 0.0;
    }
    Framework & f = Framework::instance();
    f.m_stat_distance_cm.addNewValueDeg(m_distance*100);
}

GpsPoint_ptr AutoPathModule::getPointBefore(){
    return m_pt_before;
}
GpsPoint_ptr AutoPathModule::getPointAfter(){
    return m_pt_after;
}

void AutoPathModule::deletePath(const std::string & s){
    auto paths = m_auto_paths;
    m_auto_paths.clear();
    bool find = false;
    for(auto p : paths){
        if(!find && p->getLabel() == s){
            find = true;
        } else{
            m_auto_paths.push_back(p);
        }
    }
}

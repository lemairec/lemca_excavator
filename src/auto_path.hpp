#ifndef AUTO_PATH_MODULE_H
#define AUTO_PATH_MODULE_H

#include "util/include.hpp"
#include "util/util.hpp"
#include "gps/position_module.hpp"

struct AutoPath{
    std::string m_file_name = "";
    std::string m_name = "";
    std::string m_datetime = "";
    std::string m_datetime_fr = "";
    
    
    
    std::string m_label = "";
    std::string & getLabel(){
        if(m_label.empty()){
            init();
            m_label = m_datetime_fr + " - " + m_name;
        }
        return m_label;
    }
    
    void init();
};

class AutoPathModule {
    bool m_debug = true;
public:
    AutoPathModule();
    void initOrLoadConfig(Config & config);

    std::vector<AutoPath *> m_auto_paths;
    AutoPath * findByLabel(const std::string & label);
    
    void load();
    void save();
    void clear();
    
    std::vector<RMCFrame_ptr> m_rmcs;
    std::vector<GGAFrame_ptr> m_ggas;
    void onNewFrame(const std::string & s);
    void onNewImportantPoint(GpsPoint_ptr p);
    void onGGAFrame(GGAFrame_ptr gga);
    void onRMCFrame(RMCFrame_ptr rmc);
    void onNewPoint(GpsPoint_ptr p, double deplacementX, double deplacementY);
    void onEvent100ms();
    
    size_t m_i_min;
    
    size_t m_i_min_before;
    size_t m_i_min_after;
    std::vector<GpsPoint_ptr> m_paths;
    GpsPoint_ptr m_pt_before;
    GpsPoint_ptr m_pt_after;
    
    GpsPoint_ptr getPointBefore();
    GpsPoint_ptr getPointAfter();
    
    
    bool m_save = false;
    std::ofstream m_file_save_stream;
    std::ofstream m_file_save_debug_stream;
    void init_save(const std::string & name);
    

    
    
    
    
    size_t m_load_nb_l = 0;
    size_t m_load_i = 0;
    int m_load_progress = -1;
    std::string m_load_path;
    
    myTime m_begin;
    double m_time_ms;
    
    void beginAutoPath(const std::string & s);
    void workAutoPath();
    
    double m_distance;
    double m_diff_h;
    
    void deletePath(const std::string & s);
};

#endif // GPS_FRAMEWORK_H
